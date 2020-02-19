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

type Records struct {
	Records []Record `json:"records"`
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

func createRecord(x float64) Record {
	var record Record
	record.createRecord(x)
	return record
}

func createRecords(start float64, step float64, count int) []Record {
	records := make([]Record, 0, count)
	for i := 0; i < count; i++ {
		records = append(records, createRecord(start))
		start += step
	}
	return records
}

func (records *Records) createRecords(start float64, step float64, count int) *Records {
	records.Records = createRecords(start, step, count)
	return records
}
