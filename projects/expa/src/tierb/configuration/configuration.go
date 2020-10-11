package configuration

import (
	"fmt"
	"io/ioutil"
	"log"

	"gopkg.in/yaml.v3"
)

// Configuration structure
type Configuration struct {
	Logging *Logging `yaml:"logging"`
	Service *Service `yaml:"service"`
	Hub     *Hub     `yaml:"hub"`
}

// Logging structure export
type Logging struct {
	Path string `yaml:"path"`
}

// Service structure export
type Service struct {
	Binding *Binding `yaml:"binding"`
	Path    string   `yaml:"path"`
}

// Hub structure export
type Hub struct {
	Interval uint32 `yaml:"interval"`
}

// Binding structure export
type Binding struct {
	Address string `yaml:"address"`
	Port    uint16 `yaml:"port"`
}

// Instance export
var Instance Configuration

// Initialize function export
func Initialize(filename string) {
	yamlFile, err := ioutil.ReadFile(filename)
	if err != nil {
		log.Printf("Failed to open the configuration file: %v", err)
	}
	err = yaml.Unmarshal(yamlFile, &Instance)
	if err != nil {
		log.Fatalf("Failed to unmarshal the configuration file: %v", err)
	}
}

func formatHost(binding *Binding) string {
	return fmt.Sprintf("%s:%d", binding.Address, binding.Port)
}

// GetServiceHost function export
func (c *Configuration) GetServiceHost() string {
	return formatHost(c.Service.Binding)
}
