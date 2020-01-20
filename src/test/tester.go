package main

// #cgo LDFLAGS: -lgsswrap
// #include "test/glue.h"
// #include <stdlib.h>
import "C"
import (
	"encoding/binary"
	"encoding/hex"
	"flag"
	"fmt"
	"log"
	"net"
	"os"
	"unsafe"
)

var (
	addr          string
	con           *net.Conn
	userdata      string
	fwdAddr       string
	fwdServerName string
	fwdHostbased  bool
)

func main() {
	server := flag.Bool("server", false,
		"set to run as server, false to run as client")
	tmpAddr := flag.String("address", "",
		"mandatory network address (host:port)")
	serverName := flag.String("servername", "",
		"mandatory server name")
	hostbased := flag.Bool("hostbased", false,
		"true for GSS_C_NT_HOSTBASED_SERVICE server name principal, "+
			"false for GSS_C_NT_USERNAME server name principal")
	clientName := flag.String("clientname", "",
		"client name (not applicable for server)")
	clientPass := flag.String("clientpass", "",
		"client password (optional, and not applicable for server)")
	tmpFwdAddr := flag.String("forward", "",
		"forwarding network address using delegated client credential")
	tmpFwdServerName := flag.String("forward-servername", "",
		"forwarding server name using delegated client credentials")
	tmpFwdHostbased := flag.Bool("forward-hostbased", false,
		"true for GSS_C_NT_HOSTBASED_SERVICE forward-servername principal, "+
			"false for GSS_C_NT_USERNAME forward-servername principal")

	flag.Parse()

	addr = *tmpAddr
	fwdAddr = *tmpFwdAddr
	fwdServerName = *tmpFwdServerName
	fwdHostbased = *tmpFwdHostbased

	if len(addr) == 0 || len(*serverName) == 0 {
		flag.Usage()
		os.Exit(1)
	}

	if *server && (*clientName != "" || *clientPass != "") {
		log.Fatal("clientname and clientpass are not applicable for server")
	}

	if !*server && (fwdAddr != "" || fwdServerName != "") {
		log.Fatal("forward address and forward-servername are not " +
			"applicable for client")
	}

	if (fwdAddr == "") != (fwdServerName == "") {
		log.Fatal("forward address and forward-servername are both required")
	}

	cred := C.gsswrap_make_credential()
	defer C.gsswrap_destroy_credential(cred)

	cservername := C.CString(*serverName)
	defer C.free(unsafe.Pointer(cservername))

	if *server {
		runServer(cred, cservername, *hostbased)
	} else {
		runClient(cred, cservername, *hostbased, clientName, clientPass)
	}
}

func runServer(
	cred *C.struct_gsswrap_credential,
	cservername *C.char,
	hostbased bool) {

	log.Print("Running as server on ", addr)

	if !C.gsswrap_set_server_cred(cred, cservername, hostbased == true) {
		log.Fatal("Failed to set server credential - ",
			C.gsswrap_last_credential_error(cred))
	}

	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatal("Error trying to listen on ", addr, " - ", err)
	}

	connectionNumber := 0

	for {
		tmpcon, err := listener.Accept()
		if err != nil {
			log.Fatal("Failed to accept - ", err)
		}
		con = &tmpcon
		ctx := C.glue_make_context()
		userdata = fmt.Sprintf("server connection %d", connectionNumber)
		cuserdata := C.CString(userdata)

		if C.gsswrap_accept(cred, ctx, unsafe.Pointer(cuserdata)) {
			log.Print("success ->",
				C.GoString(C.gsswrap_client_principal(ctx)), "<-")
			logFlags(ctx)
			forwardDelegated(ctx)
			sendAndReceiveSomeData("server", "client")
		} else {
			log.Print("gsswrap_accept failure ",
				C.GoString(C.gsswrap_last_context_error(ctx)))
		}

		C.free(unsafe.Pointer(cuserdata))
		C.gsswrap_destroy_context(ctx)
		(*con).Close()
	}
}

func forwardDelegated(ctx *C.struct_gsswrap_context) {
	if fwdAddr == "" || fwdServerName == "" {
		return
	}
	if C.gsswrap_delegated(ctx) == false {
		log.Fatal("client credential was not delegated")
	}

	cred := C.gsswrap_make_credential()
	if C.gsswrap_set_client_cred_delegated(cred, ctx) == false {
		log.Fatal("failed to create delegated client credential")
	}

	taddr := addr
	tcon := con
	defer func() {
		addr = taddr
		con = tcon
	}()

	addr = fwdAddr
	con = nil

	cfwdservername := C.CString(fwdServerName)
	defer C.free(unsafe.Pointer(cfwdservername))

	exchangeWithServer(cred, cfwdservername, fwdHostbased)
}

func runClient(
	cred *C.struct_gsswrap_credential,
	cservername *C.char,
	hostbased bool,
	clientName *string,
	clientPass *string) {

	log.Print("Running as client on ", addr)

	if !setClientCred(cred, clientName, clientPass) {
		log.Fatal("Failed to set client credential")
	}

	exchangeWithServer(cred, cservername, hostbased)
}

func setClientCred(
	cred *C.struct_gsswrap_credential,
	clientName *string,
	clientPass *string) bool {
	if *clientName == "" {
		C.gsswrap_set_client_cred_default(cred)
		return true
	}
	cclientname := C.CString(*clientName)
	defer C.free(unsafe.Pointer(cclientname))
	if *clientPass == "" {
		if !C.gsswrap_set_client_cred(cred, cclientname) {
			log.Print(C.GoString(C.gsswrap_last_credential_error(cred)))
			return false
		}
	} else {
		cclientpass := C.CString(*clientPass)
		defer C.free(unsafe.Pointer(cclientpass))
		if !C.gsswrap_set_client_cred_pw(cred, cclientname, cclientpass) {
			log.Print(C.GoString(C.gsswrap_last_credential_error(cred)))
			return false
		}
	}
	return true
}

func exchangeWithServer(
	cred *C.struct_gsswrap_credential,
	cservername *C.char,
	hostbased bool) {
	defer func() {
		if con != nil {
			(*con).Close()
		}
	}()

	if !C.gsswrap_set_server_name(cred, cservername, hostbased == true) {
		log.Fatal("Failed to set server name - ",
			C.gsswrap_last_credential_error(cred))
	}

	ctx := C.glue_make_context()
	defer C.gsswrap_destroy_context(ctx)
	userdata = "client connection"
	cuserdata := C.CString(userdata)
	defer C.free(unsafe.Pointer(cuserdata))

	if C.gsswrap_initiate(cred, ctx, unsafe.Pointer(cuserdata)) {
		log.Print("succes")
		logFlags(ctx)
		sendAndReceiveSomeData("client", "server")
	} else {
		log.Print("gsswrap_initiate failure ",
			C.GoString(C.gsswrap_last_context_error(ctx)))
	}
}

func logFlags(ctx *C.struct_gsswrap_context) {
	log.Print("confidentiality available: ",
		C.gsswrap_confidentiality_available(ctx) == true)
	log.Print("    credential delegation: ",
		C.gsswrap_delegated(ctx) == true)
	log.Print("      integrity available: ",
		C.gsswrap_integrity_available(ctx) == true)
	log.Print("    mutual authentication: ",
		C.gsswrap_mutual_auth(ctx) == true)
	log.Print("out of sequence detection: ",
		C.gsswrap_out_of_sequence_detection(ctx) == true)
	log.Print("         replay detection: ",
		C.gsswrap_replay_detection(ctx) == true)
}

func setupConnection() bool {
	if con == nil {
		c, err := net.Dial("tcp", addr)
		if err != nil {
			log.Print("Error trying to connect to ", addr, " - ", err)
			return false
		}

		con = &c
	}
	return true
}

func sendAndReceiveSomeData(from, to string) {
	expectedReceived := "hello from " + to
	fromPayload := []byte("hello from " + from)
	sendToPeer(C.size_t(len(fromPayload)), unsafe.Pointer(&fromPayload[0]))
	len, bufptr := recvFromPeer()
	val := C.GoString((*C.char)(bufptr))
	C.free(unsafe.Pointer(bufptr))
	log.Print("received ", len, " bytes: ", val)
	if val != expectedReceived {
		log.Fatal("Did not receive expected: ", expectedReceived)
	}
}

//export sendToPeer
func sendToPeer(length C.size_t, data unsafe.Pointer) bool {
	if !setupConnection() {
		return false
	}

	log.Print("Sending ", length, " bytes to peer")
	err := binary.Write(*con, binary.LittleEndian, length)
	if err != nil {
		log.Fatal("Error sending length to peer - ", err)
	}

	d := C.GoBytes(data, C.int(length))
	log.Print("--==>\n", hex.Dump(d))
	if len(d) != int(length) {
		log.Fatal("Incorrect length of byte buffer")
	}

	err = binary.Write(*con, binary.LittleEndian, d)
	if err != nil {
		log.Fatal("Error sending data to peer - ", err)
	}
	return true
}

// returned pointer has to be C.free()'d
//export recvFromPeer
func recvFromPeer() (C.size_t, unsafe.Pointer) {
	log.Print("Receiving from peer")
	var length C.size_t
	err := binary.Read(*con, binary.LittleEndian, &length)
	if err != nil {
		log.Fatal("Error receiving length from peer - ", err)
	}

	data := make([]byte, length)
	err = binary.Read(*con, binary.LittleEndian, data)
	if err != nil {
		log.Fatal("Error receiving data from peer - ", err)
	}
	log.Print("Received ", length, " bytes from peer")
	log.Print("<==--\n", hex.Dump(data))
	return length, C.CBytes(data) // C.CBytes has to be C.free()'d
}

//export verifyUserData
func verifyUserData(cuserdata *C.char) {
	if cuserdata == nil || userdata != C.GoString(cuserdata) {
		log.Fatal("mismatched userdata, expecting ", userdata)
	}
}
