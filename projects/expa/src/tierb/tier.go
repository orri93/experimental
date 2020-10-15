package main

// #cgo CFLAGS: -I../../include -I../../extern/gosc/include
// #cgo LDFLAGS: -L../../lib -ltiera -lgosc -lm
// #include <stdlib.h>
// #include <tiera/tiera.h>
import "C"
import (
	"flag"
	"fmt"
	"io"
	"log"
	"os"
	"unsafe"

	"./application"
	"./application/model"
	"./configuration"
)

var yamlConfigurationFile = flag.String("config", "configuration.yaml", "configuration")
var webPath = flag.String("web", "", "web path")

var logFile *os.File

// initialize the data model
func initialize(count uint32, dataPointCount uint32, majorCount uint32) {
	flag.Parse()

	fmt.Println("Reading configuration from '" + *yamlConfigurationFile + "'")
	configuration.Initialize(*yamlConfigurationFile)

	logFilePath := configuration.Instance.Logging.Path
	fmt.Println("Using '" + logFilePath + "' as log file")

	flag := os.O_RDWR | os.O_CREATE | os.O_APPEND
	logFile, err := os.OpenFile(logFilePath, flag, 0666)
	if err != nil {
		log.Fatalf("Error opening log file '%s': %v", logFilePath, err)
	}

	mw := io.MultiWriter(os.Stdout, logFile)
	log.SetOutput(mw)

	log.Println("Starting Experiment A Tier Service")

	tc := C.int(count)
	dpc := C.int(dataPointCount)
	mc := C.int(majorCount)
	cResult := C.gos_tier_a_initialize(tc, dpc, mc)
	result := bool(cResult)
	if result {
		model.Initialize(count, dataPointCount)
		log.Println("Simulated data initialized")

		cd := C.double(0.0)
		cv := C.double(0.0)
		for i := uint32(0); i < dataPointCount; i++ {
			for j := uint32(0); j < count; j++ {
				ci := C.int(i)
				cj := C.int(j)
				cResult = C.gos_tier_a_get_depth(&cd, cj, ci)
				ra := bool(cResult)
				cResult = C.gos_tier_a_get_data(&cv, cj, ci)
				rb := bool(cResult)
				if ra && rb {
					depth := float64(cd)
					value := float64(cv)
					model.Data.Set(i, j, depth, value)
				}
			}
		}

		xFrom, xTo := C.double(0.0), C.double(0.0)
		yFrom, yTo := C.double(0.0), C.double(0.0)
		cResult = C.gos_tier_a_get_depth_range(&xFrom, &xTo)
		ra := bool(cResult)
		cResult = C.gos_tier_a_get_data_range(&yFrom, &yTo)
		rb := bool(cResult)
		if ra && rb {
			model.Data.Matrix.Ranges.X.F = float64(xFrom)
			model.Data.Matrix.Ranges.X.T = float64(xTo)
			model.Data.Matrix.Ranges.Y.F = float64(yFrom)
			model.Data.Matrix.Ranges.Y.T = float64(yTo)
		}

		C.gos_tier_a_shutdown()
		log.Println("Simulated data cleared")
	} else {
		cMessageLength := C.int(0)
		var cMessage *C.char
		cMessage = C.gos_tier_a_message_length(&cMessageLength)
		message := C.GoBytes(unsafe.Pointer(cMessage), cMessageLength)
		log.Fatalln("Initializing simulated data failed " + string(message))
	}
}

// shutdown the data model
func shutdown() {
	log.Println("Close log file")
	logFile.Close()
}

func main() {
	initialize(3, 1024, 32)
	defer shutdown()

	application := &application.Application{}
	application.Initialize(*webPath)
	application.Execute()
}
