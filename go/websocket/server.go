package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"math"
	"net/http"
	"strconv"
	"time"

	"github.com/gorilla/mux"

	"gopkg.in/yaml.v2"

	"github.com/gorilla/websocket"
	"github.com/justinas/alice"
)

var err error

var start time.Time

const yamlconfigurationfile string = "server.yaml"

type Configuration struct {
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

type Records struct {
	Records []Record `json:"records"`
}

var configuration Configuration

func logMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		log.Println(r.URL.Path)
		next.ServeHTTP(w, r)
	})
}

func (record *Record) createRecord(x float64) *Record {
	record.X = x
	record.Y = math.Sin(x)
	return record
}

func createRecord(x float64) Record {
	var record Record
	record.createRecord(x)
	return record
}

func createRecords(start float64, step float64, count int) []Record {
	records := make([]Record, 0, count)
	for i := 0; i < count; i++ {
		records = append(records, createRecord(start))
		start += step
	}
	return records
}

func (records *Records) createRecords(start float64, step float64, count int) *Records {
	records.Records = createRecords(start, step, count)
	return records
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

func createRecordsPayload(start float64, step float64, count int) []byte {
	var records Records
	records.createRecords(start, step, count)
	jr, err := json.Marshal(records)
	if err != nil {
		log.Printf("Failed to marchal records")
	}
	jrs := string(jr)
	return []byte(jrs)
}

func getAll(w http.ResponseWriter, r *http.Request) {
	payload := createRecordsPayload(0.0, 0.1, 64)
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	w.Write(payload)
}

func getRecords(w http.ResponseWriter, r *http.Request) {
	pathParams := mux.Vars(r)
	w.Header().Set("Content-Type", "application/json")

	count := -1
	var err error
	if val, ok := pathParams["count"]; ok {
		count, err = strconv.Atoi(val)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			w.Write([]byte(`{"message": "need a number"}`))
			return
		}
	}

	payload := createRecordsPayload(0.0, 0.1, count)

	w.Write(payload)
}

func main() {
	start = time.Now()

	configuration.getConfiguration()

	fmt.Println("Port:", configuration.Port)
	fmt.Println("Delay:", configuration.Delay)

	router := mux.NewRouter()
	api := router.PathPrefix("/api/v1").Subrouter()
	api.HandleFunc("", getAll).Methods(http.MethodGet)
	api.HandleFunc("/records/{count}", getRecords).Methods(http.MethodGet)

	logger := alice.New(logMiddleware)
	http.Handle("/ws", logger.Then(http.HandlerFunc(wsHandler)))
	http.ListenAndServe(fmt.Sprintf(":%d", configuration.Port), router)
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
	wrapper.Record.createRecord(float64(elapsedns) / 1000000000)
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
