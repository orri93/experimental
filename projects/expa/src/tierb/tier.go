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
	"./configuration"
)

var yamlConfigurationFile = flag.String("config", "configuration.yaml", "configuration")
var webPath = flag.String("web", "", "web path")

var logFile *os.File
var model model.Model

// initialize the data model
func initialize(toolcount uint32, datapointcount uint32, majorcount uint32) {
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

	tc := C.int(toolcount)
	dpc := C.int(datapointcount)
	mc := C.int(majorcount)
	cresult := C.gos_tier_a_initialize(tc, dpc, mc)
	result := bool(cresult)
	if result {
		model.Model.Initialize(toolcount, datapointcount)
		log.Println("Simulated data initialized")
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
	C.gos_tier_a_shutdown()
	log.Println("Simulated data cleared")

	logFile.Close()
}

func main() {
	initialize(3, 1024, 32)
	defer shutdown()

	application := &application.Application{}
	application.Initialize(*webPath)
	application.Execute()
}
