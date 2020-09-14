package app

import (
	"net/http"

	"github.com/gorilla/mux"
)

type App struct {
	Router *mux.Router
}

func (a *App) Initialize(configuration *config.Configuration) {
	a.Router = mux.NewRouter()
	a.setRouters()
}

func (a *App) setRouters() {
	a.Get("/ecd", a.handleRequest())
}

func (a *App) Get(path string, f func(w http.ResponseWriter, r *http.Request)) {

}

type RequestHandlerFunction func(w http.ResponseWriter, r *http.Request)

func (a *App) handleRequest(handler RequestHandlerFunction) http.HandleFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		handler(w, r)
	}
}
