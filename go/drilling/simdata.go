package simdata

// #cgo CFLAGS: -I../../include
// #cgo LDFLAGS: -L../../lib -lgos -lgosdrilling -Wl,-rpath=../../lib
// #include <stdlib.h>
// #include <gos/drilling/density.h>

import "C"
import "log"

func init(uint32 toolcount, uint32 datapointcount, uint32 majorcount) bool {
	tc := C.int(toolcount)
	dpc := C.int(datapointcount)
	mc := C.int(majorcount)
	result := C.gos_exp_drilling_density_initialize(tc, dpc, mc)
	if result > c.int(0) {
		log.Println("Drilling density data initialized")
		return true
	} else {
		cmlength = C.int(0)
		cmessage := C.gos_exp_drilling_message_length(&mlength)
		message := C.GoBytes(cmessage, cmlength)
		log.Println("Initializing drilling density data failed " + message)
		return false
}

func shutdown() {
	C.gos_exp_drilling_density_shutdown()
	log.Println("Drilling density data cleared")
}
