package configuration

import (
	"io/ioutil"
	"log"

	"gopkg.in/yaml.v2"
)

// Configuration structure export
type Configuration struct {
	Path  string `yaml:"path"`
	Port  uint16 `yaml:"port"`
	Delay uint32 `yaml:"delay"`
}

func (configuration *Configuration) Get(filename string) *Configuration {
	yamlfile, err := ioutil.ReadFile(filename)
	if err != nil {
		log.Printf("Failed to open the configuration file: %v", err)
	}
	err = yaml.Unmarshal(yamlfile, configuration)
	if err != nil {
		log.Fatalf("Failed to unmarshal the configuration file: %v", err)
	}
	return configuration
}
