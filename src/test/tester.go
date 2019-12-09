package main

// #cgo LDFLAGS: -lgsswrap
// #include "test/glue.h"
import "C"
import (
	"encoding/binary"
	"encoding/hex"
	"flag"
	"log"
	"net"
	"unsafe"
)

func main() {
	server := flag.Bool("server", false,
		"set to run as server, false to run as client")
	addr := flag.String("address", "",
		"network address (host:port)")
	flag.Parse()

	cred := C.gsswrap_make_credential()
	defer C.gsswrap_destroy_credential(cred)

	if *server {
		runServer(addr, cred)
	} else {
		runClient(addr, cred)
	}
}

var con net.Conn

func runServer(addr *string, cred *C.struct_gsswrap_credential) {
	log.Print("Running as server on ", *addr)
	listener, err := net.Listen("tcp", *addr)
	if err != nil {
		log.Fatal("Error trying to listen on ", *addr, " - ", err)
	}

	for {
		con, err = listener.Accept()
		if err != nil {
			log.Fatal("Failed to accept - ", err)
		}

		ctx := C.glue_make_context()
		defer C.gsswrap_destroy_context(ctx)

		con.Close()
	}
}

func runClient(addr *string, cred *C.struct_gsswrap_credential) {
	log.Print("Running as client on ", *addr)
	c, err := net.Dial("tcp", *addr)
	if err != nil {
		log.Fatal("Error trying to connet to ", *addr, " - ", err)
	}

	con = c

	con.Close()
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
