package model

import "math/rand"

const matrixDummyVectorCount int = 32
const vectorDummyPointCount int = 3

// CreateDummyMatrix function creates a dummy matrix
func CreateDummyMatrix() *Matrix {
	matrix := Matrix{Vectors: make([]Vector, matrixDummyVectorCount)}
	for i := 0; i < matrixDummyVectorCount; i++ {
		matrix.Vectors[i].Points = make([]Point, vectorDummyPointCount)
		for j := 0; j < vectorDummyPointCount; j++ {
			matrix.Vectors[i].Points[j].X = rand.Float64()
			matrix.Vectors[i].Points[j].Y = rand.Float64()
		}
	}
	return &matrix
}

// CreateDummyVector function creates a vector
func CreateDummyVector() *Vector {
	vector := Vector{Points: make([]Point, vectorDummyPointCount)}
	for i := 0; i < vectorDummyPointCount; i++ {
		vector.Points[i].X = rand.Float64()
		vector.Points[i].Y = rand.Float64()
	}
	return &vector
}
