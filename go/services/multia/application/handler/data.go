package handler

import (
	"../model"
	"fmt"
	"github.com/gorilla/mux"
	"log"
	"math/rand"
	"net/http"
	"strconv"
)

const matrixVectorCount int = 32
const vectorNumberCount int = 3

// CreateMatrix function creates a matrix
func CreateMatrix() *model.Matrix {
	matrix := model.Matrix{Vectors: make([]model.Vector, matrixVectorCount)}
	for i := 0; i < matrixVectorCount; i++ {
		matrix.Vectors[i].Values = make([]float64, vectorNumberCount)
		for j := 0; j < vectorNumberCount; j++ {
			matrix.Vectors[i].Values[j] = rand.Float64()
		}
	}
	return &matrix
}

// CreateVector function creates a vector
func CreateVector() *model.Vector {
	vector := model.Vector{Values: make([]float64, vectorNumberCount)}
	for i := 0; i < vectorNumberCount; i++ {
		vector.Values[i] = rand.Float64()
	}
	return &vector
}

// GetMatrix returns the matrix
func GetMatrix(w http.ResponseWriter, r *http.Request) {
	log.Println("Servicing a Matrix Rest for " + r.URL.Path)
	matrix := CreateMatrix()
	respondJSON(w, http.StatusOK, matrix)
}

// GetVector returns a Vector
func GetVector(w http.ResponseWriter, r *http.Request) {
	log.Println("Servicing a Vector Rest for " + r.URL.Path)
	vars := mux.Vars(r)
	idText := vars["id"]
	id, err := strconv.Atoi(idText)
	if err != nil {
		respondError(w, http.StatusBadRequest, err.Error())
		return
	}
	if id >= 0 && id < matrixVectorCount {
		vector := CreateVector()
		respondJSON(w, http.StatusOK, vector)
	} else {
		error := fmt.Sprintf("The id %d is out of range", id)
		respondError(w, http.StatusNotFound, error)
	}
}
