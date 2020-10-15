package rest

import (
	"log"
	"net/http"
	"strconv"

	"github.com/gorilla/csrf"
	"github.com/gorilla/mux"

	//"github.com/gorilla/sessions"

	"../../model"
)

//var store = sessions.NewCookieStore

// HandleMatrixRequest function export
func HandleMatrixRequest(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	countText := vars["count"]
	count, err := strconv.Atoi(countText)
	if err != nil {
		log.Printf("Failed to convert '%s' to integer: %v", countText, err)
		respondError(w, http.StatusBadRequest, err.Error())
		return
	}

	log.Printf("Handle Matrix Request with count of %d", count)
	matrix := model.Data.GetSubMatrix(uint32(count))

	// Get the token and pass it in the CSRF header. Our JSON-speaking client
	// or JavaScript framework can now read the header and return the token in
	// in its own "X-CSRF-Token" request header on the subsequent POST.
	w.Header().Set("X-CSRF-Token", csrf.Token(r))

	respondJSON(w, http.StatusOK, matrix)
}
