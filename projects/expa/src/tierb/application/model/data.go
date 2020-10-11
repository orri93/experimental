package model

// #cgo CFLAGS: -I../../include
// #cgo LDFLAGS: -L../../lib -lgosc -ltiera -Wl,-rpath=../../lib
// #include <stdlib.h>
// #include <tiera/tiera.h>

import (
	"C"
	"log"
)

// Initialize the data model
func Initialize(uint32 toolcount, uint32 datapointcount, uint32 majorcount) bool {
	tc := C.int(toolcount)
	dpc := C.int(datapointcount)
	mc := C.int(majorcount)
	result := C.gos_tier_a_initialize(tc, dpc, mc)
	if result > c.int(0) {
		log.Println("Simulated data initialized")
		return true
	}
	cMessageLength = C.int(0)
	cMessage := C.gos_tier_a_message_length(&cMessageLength)
	message := C.GoBytes(cMessage, cMessageLength)
	log.Println("Initializing simulated data failed " + message)
	return false
}

// Shutdown the data model
func Shutdown() {
	C.gos_tier_a_shutdown()
	log.Println("Simulated data cleared")
}
