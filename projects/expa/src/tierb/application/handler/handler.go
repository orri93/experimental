package handler

import (
	"log"
	"net/http"

	"github.com/gorilla/mux"

	"../../configuration"
	"./web"
)

// Handler has router
type Handler struct {
	Router *mux.Router
}

// Initialize initializes the application with predefined configuration
func (h *Handler) Initialize() {
	h.Router = mux.NewRouter()
}

// Web wraps the router for a Web File
func (h *Handler) Web(web *web.Web) {
	web.CreateRoutes(h.Router)
}

// WebSocket wraps the router for a WebSocket
func (h *Handler) WebSocket(path string, f func(w http.ResponseWriter, r *http.Request)) {
	path = configuration.Instance.Service.Path + path
	log.Println("Adding '" + path + "' as WebSocket path")
	h.Router.HandleFunc(path, f)
}

// Get wraps the router for GET method
func (h *Handler) Get(path string, f func(w http.ResponseWriter, r *http.Request)) {
	path = configuration.Instance.Service.Path + path
	log.Println("Adding '" + path + "' as GET path")
	h.Router.HandleFunc(path, f).Methods("GET")
}

// Post wraps the router for POST method
func (h *Handler) Post(path string, f func(w http.ResponseWriter, r *http.Request)) {
	path = configuration.Instance.Service.Path + path
	log.Println("Adding '" + path + "' as POST path")
	h.Router.HandleFunc(path, f).Methods("POST")
}

// Put wraps the router for PUT method
func (h *Handler) Put(path string, f func(w http.ResponseWriter, r *http.Request)) {
	path = configuration.Instance.Service.Path + path
	log.Println("Adding '" + path + "' as PUT path")
	h.Router.HandleFunc(path, f).Methods("PUT")
}

// Delete wraps the router for DELETE method
func (h *Handler) Delete(path string, f func(w http.ResponseWriter, r *http.Request)) {
	path = configuration.Instance.Service.Path + path
	log.Println("Adding '" + path + "' as DELETE path")
	h.Router.HandleFunc(path, f).Methods("DELETE")
}

// Execute the application on it's router
func (h *Handler) Execute() {
	host := configuration.Instance.GetServiceHost()
	welcome := "Starting to service " + host + " with root path as " + configuration.Instance.Service.Path
	log.Println(welcome)
	log.Fatal(http.ListenAndServe(host, h.Router))
}
