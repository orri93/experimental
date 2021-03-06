package main

import (
	"context"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"os/signal"
	"strconv"
	"time"

	"github.com/gorilla/mux"

	"github.com/gorilla/websocket"
)

var err error

var start time.Time

const yamlconfigurationfile string = "server.yaml"

var configuration Configuration

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

func getIndex(w http.ResponseWriter, r *http.Request) {
	indexfile, err := ioutil.ReadFile("index.html")
	if err != nil {
		log.Printf("Failed to read index.html")
	}
	w.Write(indexfile)
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

func wsHandler(w http.ResponseWriter, r *http.Request) {
	conn, err := websocket.Upgrade(w, r, w.Header(), 1024, 1024)
	if err != nil {
		http.Error(w, "Could not open websocket connection", http.StatusBadRequest)
	}
	go echo(conn)
}

func main() {
	start = time.Now()

	configuration.getConfiguration()

	fmt.Println("Path:", configuration.Path)
	fmt.Println("Address:", configuration.Address)
	fmt.Println("Port:", configuration.Port)
	fmt.Println("Delay:", configuration.Delay)
	fmt.Println("Idle:", configuration.Idle)
	fmt.Println("Timeout:", configuration.Timeout)

	var wait time.Duration
	wait = time.Duration(configuration.Timeout) * time.Second

	var webdir string
	webdir = configuration.Path

	router := mux.NewRouter()
	api := router.PathPrefix("/api/v1").Subrouter()
	api.HandleFunc("", getAll).Methods(http.MethodGet)
	api.HandleFunc("/records/{count}", getRecords).Methods(http.MethodGet)

	// This will serve files under /<filename>
	router.PathPrefix("/").Handler(http.StripPrefix("/", http.FileServer(http.Dir(webdir))))

	address := fmt.Sprintf("%s:%d", configuration.Address, configuration.Port)

	srv := &http.Server{
		Addr:         address,
		WriteTimeout: time.Second * time.Duration(configuration.Timeout),
		ReadTimeout:  time.Second * time.Duration(configuration.Timeout),
		IdleTimeout:  time.Second * time.Duration(configuration.Idle),
		Handler:      router,
	}

	go func() {
		if err := srv.ListenAndServe(); err != nil {
			log.Println(err)
		}
	}()

	c := make(chan os.Signal, 1)
	// We'll accept graceful shutdowns when quit via SIGINT (Ctrl+C)
	// SIGKILL, SIGQUIT or SIGTERM (Ctrl+/) will not be caught.
	signal.Notify(c, os.Interrupt)

	// Block until we receive our signal.
	<-c

	// Create a deadline to wait for.
	ctx, cancel := context.WithTimeout(context.Background(), wait)
	defer cancel()
	// Doesn't block if no connections, but will otherwise wait
	// until the timeout deadline.
	srv.Shutdown(ctx)
	// Optionally, you could run srv.Shutdown in a goroutine and block on
	// <-ctx.Done() if your application should wait for other services
	// to finalize based on context cancellation.
	log.Println("shutting down")
	os.Exit(0)
}
