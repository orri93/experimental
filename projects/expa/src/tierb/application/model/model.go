package model

// Range structure export
type Range struct {
	F float64 `json:"f"`
	T float64 `json:"t"`
}

// Point structure export
type Point struct {
	X float64 `json:"x"`
	Y float64 `json:"y"`
}

// Vector structure export
type Vector struct {
	Points []Point `json:"p"`
}

// Ranges structure export
type Ranges struct {
	X Range `json:"x"`
	Y Range `json:"y"`
}

// Matrix structure export
type Matrix struct {
	Ranges  Ranges   `json:"r"`
	Vectors []Vector `json:"v"`
}

type WsStart struct {
	From uint32 `json:"f"`
}

type WsUpdate struct {
	At     uint32 `json:"t"`
	Vector Vector `json:"v"`
}

type WsMessage struct {
	Type    string    `json:"t"`
	Message *string   `json:"m"`
	Start   *WsStart  `json:"s"`
	Update  *WsUpdate `json:"u"`
}
