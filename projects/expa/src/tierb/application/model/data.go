package model

import "math"

// Model structure export
type Model struct {
	Matrix         Matrix `json:"matrix"`
	Count          uint32 `json:"count"`
	DataPointCount uint32 `json:"dataPointCount"`
}

// Data export
var Data Model

func min(a, b uint32) uint32 {
	if a < b {
		return a
	}
	return b
}

// Initialize the data model
func Initialize(count uint32, dataPointCount uint32) {
	Data.Matrix.initialize(count, dataPointCount)
	Data.Count = count
	Data.DataPointCount = dataPointCount
}

// CreateMatrix function export
func CreateMatrix(count uint32, dataPointCount uint32) *Matrix {
	matrix := new(Matrix)
	matrix.initialize(count, dataPointCount)
	return matrix
}

func (matrix *Matrix) initialize(count uint32, dataPointCount uint32) {
	matrix.Vectors = make([]Vector, dataPointCount)
	for i := uint32(0); i < dataPointCount; i++ {
		matrix.Vectors[i].Points = make([]Point, count)
	}
}

// CreateVector function export
func CreateVector(count uint32) *Vector {
	vector := new(Vector)
	vector.initialize(count)
	return vector
}

func (vector *Vector) initialize(count uint32) {
	vector.Points = make([]Point, count)
}

// Set to Model
func (model *Model) Set(it uint32, iv uint32, x float64, y float64) {
	model.Matrix.Set(it, iv, x, y)
}

// Get from Model
func (model *Model) Get(it uint32, iv uint32) (float64, float64) {
	return model.Matrix.Get(it, iv)
}

// Set to Matrix
func (matrix *Matrix) Set(it uint32, iv uint32, x float64, y float64) {
	matrix.Vectors[it].Points[iv].X = x
	matrix.Vectors[it].Points[iv].Y = y
}

// Get from Matrix
func (matrix *Matrix) Get(it uint32, iv uint32) (float64, float64) {
	return matrix.Vectors[it].Points[iv].X, matrix.Vectors[it].Points[iv].Y
}

// Set value in Vector
func (vector *Vector) Set(i uint32, x float64, y float64) {
	vector.Points[i].X = x
	vector.Points[i].Y = y
}

func (r *Range) initialize() {
	r.F = math.MaxFloat64
	r.T = -math.MaxFloat64
}

func (r *Range) update(v float64) {
	if v < r.F {
		r.F = v
	}
	if v > r.T {
		r.T = v
	}
}

// GetSubMatrix function export
func (model *Model) GetSubMatrix(count uint32) *Matrix {
	use := min(model.DataPointCount, count)
	matrix := CreateMatrix(model.Count, use)
	matrix.Ranges.X.initialize()
	matrix.Ranges.Y.initialize()
	for i := uint32(0); i < use; i++ {
		for j := uint32(0); j < model.Count; j++ {
			x, y := model.Get(i, j)
			matrix.Ranges.X.update(x)
			matrix.Ranges.Y.update(y)
			matrix.Set(i, j, x, y)
		}
	}
	return matrix
}

// CreateWsUpdate function export
func (model *Model) CreateWsUpdate(at uint32) *WsUpdate {
	vector := CreateVector(model.Count)

	for i := uint32(0); i < model.Count; i++ {
		x, y := model.Get(at, i)
		vector.Set(i, x, y)
	}

	update := new(WsUpdate)
	update.Vector = *vector
	update.At = at
	return update
}

// CreateWsMessage function export
func CreateWsMessage(t string) *WsMessage {
	message := new(WsMessage)
	message.Type = t
	return message
}

// CreateWsTextMessage function export
func CreateWsTextMessage(t string, m string) *WsMessage {
	message := CreateWsMessage(t)
	message.Message = &m
	return message
}
