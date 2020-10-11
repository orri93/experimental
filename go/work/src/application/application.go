package main

import (
	"configuration"
	"fmt"
)

const yamlconfigurationfile string = "configuration.yaml"

func main() {
	var configuration = configuration.Get(yamlconfigurationfile)
	fmt.Println("Hello")
}
