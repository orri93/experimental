package main

import (
	"context"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/signal"
	"time"

	"github.com/justinas/alice"

	"github.com/gorilla/websocket"
)

var err error

var start time.Time

const yamlconfigurationfile string = "ws.yaml"

var configuration Configuration

func echo(wsconn *websocket.Conn) {
	var wrapper Wrapper
	for {
		wrapper.createWrapper()
		if err = wsconn.WriteJSON(wrapper); err != nil {
			log.Println(err)
		}
		delay := time.Duration(configuration.Delay) * time.Millisecond
		time.Sleep(delay)
	}
}

func wsHandler(w http.ResponseWriter, r *http.Request) {
	wsconn, err := websocket.Upgrade(w, r, w.Header(), 1024, 1024)
	if err != nil {
		http.Error(w, "Could not open websocket connection", http.StatusBadRequest)
	}
	go echo(wsconn)
}

func logMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		log.Println(r.URL.Path)
		next.ServeHTTP(w, r)
	})
}

func main() {
	start = time.Now()

	configuration.getConfiguration()

	fmt.Println("Address:", configuration.Address)
	fmt.Println("Port:", configuration.Port)
	fmt.Println("Delay:", configuration.Delay)
	fmt.Println("Idle:", configuration.Idle)
	fmt.Println("Timeout:", configuration.Timeout)

	var wait time.Duration
	wait = time.Duration(configuration.Timeout) * time.Second

	logger := alice.New(logMiddleware)

	address := fmt.Sprintf("%s:%d", configuration.Address, configuration.Port)

	fmt.Println("Service:", address)

	http.Handle("/ws", logger.Then(http.HandlerFunc(wsHandler)))

	srv := &http.Server{
		Addr:         address,
		WriteTimeout: time.Second * time.Duration(configuration.Timeout),
		ReadTimeout:  time.Second * time.Duration(configuration.Timeout),
		IdleTimeout:  time.Second * time.Duration(configuration.Idle),
		Handler:      nil,
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
