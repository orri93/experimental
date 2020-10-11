package model

// Matrix structure export
type Matrix struct {
	Vectors []Vector `json:"vectors"`
}

// Vector structure export
type Vector struct {
	Values []float64 `json:"values"`
}
