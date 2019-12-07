package main

// #cgo LDFLAGS: -lgssapi_krb5
import "C"
import (
	"fmt"
)

func main() {
	fmt.Println("hello from go")
}
