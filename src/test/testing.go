package main

/*
#cgo LDFLAGS: -lgsswrap
#include "gsswrap/client.h"
#include "gsswrap/server.h"

bool send_to_peer_cb(const void* buffer, size_t len, void* user_data)
{
	return true;
}

bool recv_from_peer_cb(void** buffer, size_t* len, void* user_data)
{
	return true;
}

void free_buffer_cb(void* buffer, size_t len, void* user_data)
{
}

struct gsswrap_context* make_context()
{
	return gsswrap_make_context(
		send_to_peer_cb, recv_from_peer_cb, free_buffer_cb);
}
*/
import "C"
import (
	"flag"
	"log"
	"net"
)

func main() {
	server := flag.Bool("server", false,
		"set to run as server, false to run as client")
	addr := flag.String("address", "",
		"network address (host:port)")

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

		ctx := C.make_context()
		defer C.gsswrap_destroy_context(ctx)

		con.Close()
	}
}

func runClient(addr *string, cred *C.struct_gsswrap_credential) {
	log.Print("Running as client on ", *addr)
	c, err := net.Dial("tcp", *addr)
	if err != nil {
		log.Fatal("Error trying to ", *addr, " - ", err)
	}

	con = c

	con.Close()
}
