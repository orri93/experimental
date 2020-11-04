package model

// Golang basics - writing unit tests
// See https://blog.alexellis.io/golang-writing-unit-tests

import (
	"encoding/json"
	"strings"
	"testing"
)

func TestRange(t *testing.T) {
	r := new(Range)
	r.F = 10.0
	r.T = 20.0
	ejm := "{\"f\":10,\"t\":20}"
	jm, err := json.Marshal(r)
	if err != nil {
		t.Errorf("Failed to marshal a message: %v", err)
	}
	if strings.Compare(ejm, string(jm)) != 0 {
		t.Errorf("JSON was incorrect, got: '%s', want: '%s'", jm, ejm)
	}
}

func TestPoint(t *testing.T) {
	p := new(Point)
	p.X = 10.0
	p.Y = 20.0
	ejm := "{\"x\":10,\"y\":20}"
	jm, err := json.Marshal(p)
	if err != nil {
		t.Errorf("Failed to marshal a message: %v", err)
	}
	if strings.Compare(ejm, string(jm)) != 0 {
		t.Errorf("JSON was incorrect, got: '%s', want: '%s'", jm, ejm)
	}
}

func TestVector(t *testing.T) {
	const PointCount = 3
	v := new(Vector)
	v.Points = make([]Point, PointCount)
	for i := 0; i < PointCount; i++ {
		v.Points[i].X = 10.0 + float64(i)
		v.Points[i].Y = 20.0 + float64(i)
	}
	ejm := "{\"p\":[{\"x\":10,\"y\":20},{\"x\":11,\"y\":21},{\"x\":12,\"y\":22}]}"
	jm, err := json.Marshal(v)
	if err != nil {
		t.Errorf("Failed to marshal a message: %v", err)
	}
	if strings.Compare(ejm, string(jm)) != 0 {
		t.Errorf("JSON was incorrect, got: '%s', want: '%s'", jm, ejm)
	}
}
