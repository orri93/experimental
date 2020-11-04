package model

import "testing"

func TestMin(t *testing.T) {
	a := uint32(10)
	b := uint32(20)
	c := min(a, b)
	if c != a {
		t.Errorf("min was incorrect, got: %d, want: %d", c, a)
	}
	a = uint32(2)
	b = uint32(1)
	c = min(a, b)
	if c != b {
		t.Errorf("min was incorrect, got: %d, want: %d", c, b)
	}
}

func TestInitialize(t *testing.T) {
	const Count = 3
	const DataPointCount = 6
	Initialize(Count, DataPointCount)
	if Data.Count != Count {
		t.Errorf("Data Count was incorrect, go: %d, want: %d", Data.Count, Count)
	}
	if Data.DataPointCount != DataPointCount {
		t.Errorf("Data Point Count was incorrect, go: %d, want: %d", Data.DataPointCount, DataPointCount)
	}
}
