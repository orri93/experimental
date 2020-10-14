package model

// Model structure export
type Model struct {
	Matrix         Matrix `json:"matrix"`
	ToolCount      uint32 `json:"toolCount"`
	DataPointCount uint32 `json:"dataPointCount"`
}

// Initialize the data model
func (model *Model) Initialize(toolcount uint32, datapointcount uint32) {
	model.Matrix.Vectors = make([]Vector, datapointcount)
	for i := uint32(0); i < datapointcount; i++ {
		model.Matrix.Vectors[i].Points = make([]Point, toolcount)
	}
}
