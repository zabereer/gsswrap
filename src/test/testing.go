package main

// #cgo LDFLAGS: -lgsswrap
// #include "gsswrap/client.h"
// #include "gsswrap/server.h"
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

	if *server {
		runServer(addr)
	} else {
		runClient(addr)
	}
}

var con net.Conn

func runServer(addr *string) {
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

		con.Close()
	}
}

func runClient(addr *string) {
	log.Print("Running as client on ", *addr)
	c, err := net.Dial("tcp", *addr)
	if err != nil {
		log.Fatal("Error trying to ", *addr, " - ", err)
	}

	con = c

	con.Close()
}
