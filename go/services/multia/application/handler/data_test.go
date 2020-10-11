package handler

import (
	//"context"
	//"github.com/gorilla/mux"
	"net/http"
	"testing"
)

//type contextKey int
//const ( varsKey contextKey = iota )

func TestCreateMatrix(t *testing.T) {
	matrix := CreateMatrix()
	length := len(matrix.Vectors)

	if length != matrixVectorCount {
		t.Errorf("Matrix vector count was incorrect, got: %d, wanted: %d", length, matrixVectorCount)
	}

	for i := 0; i < matrixVectorCount; i++ {
		vector := matrix.Vectors[i]
		length = len(vector.Values)
		if length != vectorNumberCount {
			t.Errorf("Vector count was incorrect, got: %d, wanted: %d", length, vectorNumberCount)
		}
	}
}

func TestCreateVector(t *testing.T) {
	vector := CreateVector()
	length := len(vector.Values)
	if length != vectorNumberCount {
		t.Errorf("Vector count was incorrect, got: %d, wanted: %d", length, vectorNumberCount)
	}
}

func TestGetMatrix(t *testing.T) {
	request := http.Request{}
	tw := CreateTestWriter()

	GetMatrix(tw, &request)

	if tw.Code != http.StatusOK {
		t.Errorf("Code was incorrect, got: %d, wanted: %d", tw.Code, http.StatusOK)
	}

	//length := len(tw.Buffer)
	//expectedLength := 2275
	//if length != vectorNumberCount {
	//	t.Errorf("Response length was incorrect, got: %d, wanted: %d", length, expectedLength)
	//}
}

func TestGetVector(t *testing.T) {
	request := http.Request{}
	tw := CreateTestWriter()

	//vars := make(map[string]string)
	//vars["id"] = "10"

	//ctx := context.WithValue(request.Context(), mux.varsKey, vars)
	//requestWithContext := request.WithContext(ctx)

	GetVector(tw, &request)

	if tw.Code != http.StatusBadRequest {
		t.Errorf("Code was incorrect, got: %d, wanted: %d", tw.Code, http.StatusBadRequest)
	}
}
