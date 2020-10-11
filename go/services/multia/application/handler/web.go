package handler

import (
	"log"
	"net/http"
)

// GetHomeWeb service the home.html file
func GetHomeWeb(w http.ResponseWriter, r *http.Request) {
	log.Println("Servicing a web page for " + r.URL.Path)
	if r.Method != "GET" {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}
	http.ServeFile(w, r, "home.html")
}
