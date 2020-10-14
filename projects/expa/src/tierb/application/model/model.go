package model

// Point structure export
type Point struct {
	X float64 `json:"x"`
	Y float64 `json:"y"`
}

// Vector structure export
type Vector struct {
	Points []Point `json:"points"`
}

// Matrix structure export
type Matrix struct {
	Vectors []Vector `json:"vectors"`
}
