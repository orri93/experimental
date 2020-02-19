package main

import (
	"io/ioutil"
	"log"

	"gopkg.in/yaml.v2"
)

type Configuration struct {
	Path    string `yaml:"path"`
	Address string `yaml:"address"`
	Port    uint16 `yaml:"port"`
	Delay   uint32 `yaml:"delay"`
	Idle    uint32 `yaml:"idle"`
	Timeout uint32 `yaml:"timeout"`
}

func (configuration *Configuration) getConfiguration() *Configuration {
	yamlfile, err := ioutil.ReadFile(yamlconfigurationfile)
	if err != nil {
		log.Printf("Failed to open the configuration file: %v", err)
	}
	err = yaml.Unmarshal(yamlfile, configuration)
	if err != nil {
		log.Fatalf("Failed to unmarchal the configuration file: %v", err)
	}
	return configuration
}
