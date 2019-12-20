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

func main() {
	server := flag.Bool("server", false,
		"set to run as server, false to run as client")
	addr := flag.String("address", "",
		"mandatory network address (host:port)")
	serverName := flag.String("servername", "",
		"mandatory server name")
	hostbased := flag.Bool("hostbased", false,
		"true for GSS_C_NT_HOSTBASED_SERVICE principal, "+
			"false for GSS_C_NT_USERNAME principal")
	flag.Parse()
	if len(*addr) == 0 || len(*serverName) == 0 {
		flag.Usage()
		os.Exit(1)
	}

	cred := C.gsswrap_make_credential()
	defer C.gsswrap_destroy_credential(cred)

	cservername := C.CString(*serverName)
	defer C.free(unsafe.Pointer(cservername))

	if *server {
		runServer(addr, cred, cservername, *hostbased)
	} else {
		runClient(addr, cred, cservername, *hostbased)
	}
}

var con net.Conn
var userdata string

func runServer(
	addr *string,
	cred *C.struct_gsswrap_credential,
	cservername *C.char,
	hostbased bool) {

	log.Print("Running as server on ", *addr)

	if !C.gsswrap_set_server_cred(cred, cservername, hostbased == true) {
		log.Fatal("Failed to set server credential - ",
			C.gsswrap_last_credential_error(cred))
	}

	listener, err := net.Listen("tcp", *addr)
	if err != nil {
		log.Fatal("Error trying to listen on ", *addr, " - ", err)
	}

	connectionNumber := 0

	for {
		con, err = listener.Accept()
		if err != nil {
			log.Fatal("Failed to accept - ", err)
		}

		defer con.Close()
		ctx := C.glue_make_context()
		defer C.gsswrap_destroy_context(ctx)
		userdata = fmt.Sprintf("server connection %d", connectionNumber)
		cuserdata := C.CString(userdata)
		defer C.free(unsafe.Pointer(cuserdata))

		if C.gsswrap_accept(cred, ctx, unsafe.Pointer(cuserdata)) {
			log.Print("success")
		} else {
			log.Print("failure ", C.gsswrap_last_context_error(ctx))
		}
	}
}

func runClient(
	addr *string,
	cred *C.struct_gsswrap_credential,
	cservername *C.char,
	hostbased bool) {

	log.Print("Running as client on ", *addr)

	if !C.gsswrap_set_server_name(cred, cservername, hostbased == true) {
		log.Fatal("Failed to set server name - ",
			C.gsswrap_last_credential_error(cred))
	}

	c, err := net.Dial("tcp", *addr)
	if err != nil {
		log.Fatal("Error trying to connect to ", *addr, " - ", err)
	}

	con = c
	defer con.Close()
	ctx := C.glue_make_context()
	defer C.gsswrap_destroy_context(ctx)
	userdata = "client connection"
	cuserdata := C.CString(userdata)
	defer C.free(unsafe.Pointer(cuserdata))

	if C.gsswrap_initiate(cred, ctx, unsafe.Pointer(cuserdata)) {
		log.Print("succes")
	} else {
		log.Print("failure ", C.gsswrap_last_context_error(ctx))
	}
}

//export sendToPeer
func sendToPeer(length C.size_t, data unsafe.Pointer) bool {
	log.Print("Sending ", length, " bytes to peer")
	err := binary.Write(con, binary.LittleEndian, length)
	if err != nil {
		log.Fatal("Error sending length to peer - ", err)
	}

	d := C.GoBytes(data, C.int(length))
	log.Print(hex.Dump(d))
	if len(d) != int(length) {
		log.Fatal("Incorrect length of byte buffer")
	}

	err = binary.Write(con, binary.LittleEndian, d)
	if err != nil {
		log.Fatal("Error sending data to peer - ", err)
	}
	return true
}

//export recvFromPeer
func recvFromPeer() (C.size_t, unsafe.Pointer) {
	log.Print("Receiving from peer")
	var length C.size_t
	err := binary.Read(con, binary.LittleEndian, &length)
	if err != nil {
		log.Fatal("Error receiving length from peer - ", err)
	}

	data := make([]byte, length)
	err = binary.Read(con, binary.LittleEndian, data)
	if err != nil {
		log.Fatal("Error receiving data from peer - ", err)
	}
	log.Print("Received ", length, " bytes from peer")
	log.Print(hex.Dump(data))
	return length, C.CBytes(data) // C.CBytes has to be C.free()'d
}

//export verifyUserData
func verifyUserData(cuserdata *C.char) {
	if cuserdata == nil || userdata != C.GoString(cuserdata) {
		log.Fatal("mismatched userdata, expecting ", userdata)
	}
}
