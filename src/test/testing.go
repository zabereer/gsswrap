package main

// #cgo LDFLAGS: -lgsswrap
import "C"
import (
	"fmt"
)

func main() {
	fmt.Println("hello from go")
}
