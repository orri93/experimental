package handler

import (
	"../model"
	"net/http"
	"strings"
	"testing"
)

type TestWriter struct {
	TestHeader http.Header
	Code       int
	Buffer     []byte
}

func CreateTestWriter() *TestWriter {
	tw := new(TestWriter)
	tw.TestHeader = make(http.Header)
	return tw
}

func (tw *TestWriter) Header() http.Header {
	return tw.TestHeader
}

func (tw *TestWriter) Write(buffer []byte) (int, error) {
	tw.Buffer = buffer
	return 0, nil
}

func (tw *TestWriter) WriteHeader(statusCode int) {
	tw.Code = statusCode
}

func TestRespondJSON(t *testing.T) {
	vector := model.Vector{Values: make([]float64, vectorNumberCount)}
	for i := 0; i < vectorNumberCount; i++ {
		vector.Values[i] = float64(i + 1)
	}
	tw := CreateTestWriter()
	expectedBuffer := "{\"values\":[1,2,3]}"
	code := 1

	respondJSON(tw, code, vector)

	if tw.Code != code {
		t.Errorf("Code was incorrect, got: %d, wanted: %d", tw.Code, code)
	}

	bufferString := string(tw.Buffer)
	if strings.Compare(expectedBuffer, bufferString) != 0 {
		t.Errorf("Buffer was incorrect, got: '%s', wanted: '%s'", bufferString, expectedBuffer)
	}

	header := tw.Header()
	expectedHeaderValue := "application/json"
	headerValue := header.Get("Content-Type")
	if strings.Compare(expectedHeaderValue, headerValue) != 0 {
		t.Errorf("Header was incorrect, got: '%s', wanted: '%s'", headerValue, expectedHeaderValue)
	}
}

func TestRespondError(t *testing.T) {
	tw := CreateTestWriter()
	expectedBuffer := "{\"error\":\"Message\"}"
	message := "Message"
	code := 1

	respondError(tw, code, message)

	if tw.Code != code {
		t.Errorf("Code was incorrect, got: %d, wanted: %d", tw.Code, code)
	}

	bufferString := string(tw.Buffer)
	if strings.Compare(expectedBuffer, bufferString) != 0 {
		t.Errorf("Buffer was incorrect, got: '%s', wanted: '%s'", bufferString, expectedBuffer)
	}
}
