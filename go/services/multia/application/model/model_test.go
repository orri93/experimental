package model

import (
	"math"
	"testing"
)

const vectorNumberCount int = 3

func TestVector(t *testing.T) {
	vector := Vector{make([]float64, vectorNumberCount)}
	for i := 0; i < vectorNumberCount; i++ {
		vector.Values[i] = float64(i + 1)
	}
	length := len(vector.Values)
	if length != vectorNumberCount {
		t.Errorf("Length was incorrect, got: %d, wanted: %d", length, vectorNumberCount)
	}
	sum := 0.0
	for _, x := range vector.Values {
		sum += x
	}
	if !compare(sum, 6.0) {
		t.Errorf("Sum was incorrect, got: %f, wanted: 6.0", sum)
	}
}

func compare(a float64, b float64, d ...float64) bool {
	delta := 0.00001
	if len(d) > 0 {
		delta = d[0]
	}
	return math.Abs(a-b) < delta
}
