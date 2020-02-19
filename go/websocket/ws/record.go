package main

import (
	"math"
	"time"
)

type Wrapper struct {
	Record Record `json:"record"`
}

type Record struct {
	X float64 `json:"x"`
	Y float64 `json:"y"`
}

func (wrapper *Wrapper) createWrapper() *Wrapper {
	elapsed := time.Since(start)
	elapsedns := elapsed.Nanoseconds()
	wrapper.Record.createRecord(float64(elapsedns) / 1000000000)
	return wrapper
}

func (record *Record) createRecord(x float64) *Record {
	record.X = x
	record.Y = math.Sin(x)
	return record
}
