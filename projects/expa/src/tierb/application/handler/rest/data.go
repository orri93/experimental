package rest

import (
	"log"
	"net/http"

	"github.com/gorilla/csrf"

	"../../model"
)

// HandleMatrixRequest function export
func HandleMatrixRequest(w http.ResponseWriter, r *http.Request) {
	log.Println("Handle Matrix Request")
	matrix := model.CreateDummyMatrix()

	// Get the token and pass it in the CSRF header. Our JSON-speaking client
	// or JavaScript framework can now read the header and return the token in
	// in its own "X-CSRF-Token" request header on the subsequent POST.
	w.Header().Set("X-CSRF-Token", csrf.Token(r))

	respondJSON(w, http.StatusOK, matrix)
}
