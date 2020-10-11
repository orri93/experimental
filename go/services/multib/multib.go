package main

import (
	"./application"
	"./configuration"
	"flag"
	"fmt"
	"io"
	"log"
	"os"
)

var yamlConfigurationFile = flag.String("config", "configuration.yaml", "configuration")

func main() {
	flag.Parse()

	configuration.Initialize(*yamlConfigurationFile)

	logFilePath := configuration.Instance.Logging.Path
	fmt.Println("Using " + logFilePath + " as log file")

	flag := os.O_RDWR | os.O_CREATE | os.O_APPEND
	logFile, err := os.OpenFile(logFilePath, flag, 0666)
	if err != nil {
		log.Fatalf("Error opening log file '%s': %v", logFilePath, err)
	}
	defer logFile.Close()

	mw := io.MultiWriter(os.Stdout, logFile)
	log.SetOutput(mw)

	log.Println("Starting Multi B Experiment")

	application := &application.Application{}
	application.Initialize()
	application.Execute()
}
