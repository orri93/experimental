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
	Rest    *Rest    `yaml:"rest"`
	Socket  *Socket  `yaml:"socket"`
}

// Logging structure export
type Logging struct {
	Path string `yaml:"path"`
}

// Rest structure export
type Rest struct {
	Binding *Binding `yaml:"binding"`
	Path    string   `yaml:"path"`
}

// Socket structure export
type Socket struct {
	Binding *Binding `yaml:"binding"`
	Delay   uint32   `yaml:"delay"`
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

// GetRestHost function export
func (c *Configuration) GetRestHost() string {
	return formatHost(c.Rest.Binding)
}
