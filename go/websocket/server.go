package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"math"
	"net/http"
	"time"

	"gopkg.in/yaml.v2"

	"github.com/gorilla/websocket"
	"github.com/justinas/alice"
)

var err error

var start time.Time

const yamlconfigurationfile string = "server.yaml"

type Configuration struct {
	Path  string `yaml:"path"`
	Port  uint16 `yaml:"port"`
	Delay uint32 `yaml:"delay"`
}

type Wrapper struct {
	Record Record `json:"record"`
}

type Record struct {
	X float64 `json:"x"`
	Y float64 `json:"y"`
}

var configuration Configuration

func logMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		log.Println(r.URL.Path)
		next.ServeHTTP(w, r)
	})
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

func main() {
	start = time.Now()

	configuration.getConfiguration()

	fmt.Println("Path:", configuration.Path)
	fmt.Println("Port:", configuration.Port)
	fmt.Println("Delay:", configuration.Delay)

	serv := fmt.Sprintf(":%d", configuration.Port)

	logger := alice.New(logMiddleware)
	http.Handle(configuration.Path, logger.Then(http.HandlerFunc(wsHandler)))
	http.ListenAndServe(serv, nil)
}

func wsHandler(w http.ResponseWriter, r *http.Request) {
	conn, err := websocket.Upgrade(w, r, w.Header(), 1024, 1024)
	if err != nil {
		http.Error(w, "Could not open websocket connection", http.StatusBadRequest)
	}
	go echo(conn)
}

func (wrapper *Wrapper) createWrapper() *Wrapper {
	elapsed := time.Since(start)
	elapsedns := elapsed.Nanoseconds()
	wrapper.Record.X = float64(elapsedns) / 1000000000
	wrapper.Record.Y = math.Sin(wrapper.Record.X)
	return wrapper
}

func echo(conn *websocket.Conn) {
	var wrapper Wrapper
	for {
		wrapper.createWrapper()
		if err = conn.WriteJSON(wrapper); err != nil {
			log.Println(err)
		}
		delay := time.Duration(configuration.Delay) * time.Millisecond
		time.Sleep(delay)
	}
}
