package application

import (
	"../configuration"
	"./handler"
	"log"
	"net/http"

	"github.com/gorilla/mux"
)

// Application has router
type Application struct {
	Router *mux.Router
}

// Initialize initializes the application with predefined configuration
func (a *Application) Initialize() {
	handler.Initialize()
	go handler.Instance.Execute()

	a.Router = mux.NewRouter()
	a.setRouters()
}

// setRouters sets the all required routers
func (a *Application) setRouters() {

	a.Web("/web/home", a.handleRequest(handler.GetHomeWeb))

	a.WebSocket("/ws", a.handleRequest(handler.GetWsService))

	// Routing for handling the projects
	a.Get("/matrix", a.handleRequest(handler.GetMatrix))

	// Routing for handling the vectors
	a.Get("/vector/{id}", a.handleRequest(handler.GetVector))
}

// Web wraps the router for a Web
func (a *Application) Web(path string, f func(w http.ResponseWriter, r *http.Request)) {
	path = configuration.Instance.Rest.Path + path
	log.Println("Adding '" + path + "' as Web path")
	a.Router.HandleFunc(path, f)
}

// WebSocket wraps the router for a WebSocket
func (a *Application) WebSocket(path string, f func(w http.ResponseWriter, r *http.Request)) {
	path = configuration.Instance.Rest.Path + path
	log.Println("Adding '" + path + "' as WebSocket path")
	a.Router.HandleFunc(path, f)
}

// Get wraps the router for GET method
func (a *Application) Get(path string, f func(w http.ResponseWriter, r *http.Request)) {
	path = configuration.Instance.Rest.Path + path
	log.Println("Adding '" + path + "' as GET path")
	a.Router.HandleFunc(path, f).Methods("GET")
}

// Post wraps the router for POST method
func (a *Application) Post(path string, f func(w http.ResponseWriter, r *http.Request)) {
	a.Router.HandleFunc(configuration.Instance.Rest.Path+path, f).Methods("POST")
}

// Put wraps the router for PUT method
func (a *Application) Put(path string, f func(w http.ResponseWriter, r *http.Request)) {
	a.Router.HandleFunc(configuration.Instance.Rest.Path+path, f).Methods("PUT")
}

// Delete wraps the router for DELETE method
func (a *Application) Delete(path string, f func(w http.ResponseWriter, r *http.Request)) {
	a.Router.HandleFunc(configuration.Instance.Rest.Path+path, f).Methods("DELETE")
}

// Execute the application on it's router
func (a *Application) Execute() {
	host := configuration.Instance.GetRestHost()
	welcome := "Starting to service " + host + " with root path as " + configuration.Instance.Rest.Path
	log.Println(welcome)
	log.Fatal(http.ListenAndServe(host, a.Router))
}

// RequestHandlerFunction the Request Handler Function export
type RequestHandlerFunction func(w http.ResponseWriter, r *http.Request)

func (a *Application) handleRequest(handler RequestHandlerFunction) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		handler(w, r)
	}
}
