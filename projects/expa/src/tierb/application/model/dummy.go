package model

import "math/rand"

const matrixDummyVectorCount int = 32
const vectorDummyNumberCount int = 3

// CreateDummyMatrix function creates a dummy matrix
func CreateDummyMatrix() *Matrix {
	matrix := Matrix{Vectors: make([]Vector, matrixDummyVectorCount)}
	for i := 0; i < matrixDummyVectorCount; i++ {
		matrix.Vectors[i].Values = make([]float64, vectorDummyNumberCount)
		for j := 0; j < vectorDummyNumberCount; j++ {
			matrix.Vectors[i].Values[j] = rand.Float64()
		}
	}
	return &matrix
}

// CreateDummyVector function creates a vector
func CreateDummyVector() *Vector {
	vector := Vector{Values: make([]float64, vectorDummyNumberCount)}
	for i := 0; i < vectorDummyNumberCount; i++ {
		vector.Values[i] = rand.Float64()
	}
	return &vector
}
