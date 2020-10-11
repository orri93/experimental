package application

import (
	"./handler"
	"github.com/gorilla/mux"
)

// Application has handler
type Application struct {
	Handler handler.Handler
}

// Initialize initializes the application with predefined configuration
func (a *Application) Initialize() {
	a.Handler.Initialize()
}

// Execute the application on it's router
func (a *Application) Execute() {
}

// setRouters sets the all required routers
func (a *Application) setRouters() {

	// Routing for Web Socket
	a.Handler.WebSocket("/ws", a.Handler.HandleRequest(handler.GetWsService))

	// Routing for handling the matrix
	a.Handler.Get("/matrix", a.Handler.HandleRequest(handler.GetMatrix))

	// Routing for handling the vectors
	a.Handler.Get("/vector/{id}", a.Handler.HandleRequest(handler.GetVector))
}
