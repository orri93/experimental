package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"os"

	"./application"
	"./configuration"
)

var yamlConfigurationFile = flag.String("config", "configuration.yaml", "configuration")
var webPath = flag.String("web", "", "web path")

func main() {
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
	defer logFile.Close()

	mw := io.MultiWriter(os.Stdout, logFile)
	log.SetOutput(mw)

	log.Println("Starting Experiment A Tier Service")

	application := &application.Application{}
	application.Initialize(*webPath)
	application.Execute()
}
