package model

import (
	"math/rand"
)

const matrixVectorCount int = 32
const vectorNumberCount int = 3

// CreateMatrix function creates a matrix
func CreateMatrix() *Matrix {
	matrix := Matrix{Vectors: make([]Vector, matrixVectorCount)}
	for i := 0; i < matrixVectorCount; i++ {
		matrix.Vectors[i].Values = make([]float64, vectorNumberCount)
		for j := 0; j < vectorNumberCount; j++ {
			matrix.Vectors[i].Values[j] = rand.Float64()
		}
	}
	return &matrix
}

// CreateVector function creates a vector
func CreateVector() *Vector {
	vector := Vector{Values: make([]float64, vectorNumberCount)}
	for i := 0; i < vectorNumberCount; i++ {
		vector.Values[i] = rand.Float64()
	}
	return &vector
}
