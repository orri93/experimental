package ws

import (
	"encoding/json"
	"log"

	"../../model"
)

func (c *Client) handleStartRequest(start *model.WsStart) []byte {
	if start != nil {

		c.indexAt = start.From
		c.active = true

		outgoing := model.CreateWsMessage(TypeStarting)
		response, err := json.Marshal(outgoing)
		if err != nil {
			log.Printf("Failed to marshal Starting WebSocket response: %v", err)
		}
		return response
	} else {
		return nil
	}
}

func (c *Client) handleStopRequest() []byte {
	c.active = false
	outgoing := model.CreateWsMessage(TypeStopping)
	response, err := json.Marshal(outgoing)
	if err != nil {
		log.Printf("Failed to marshal Stopping WebSocket response: %v", err)
	}
	return response
}

func (c *Client) createUpdate() []byte {
	update := model.Data.CreateWsUpdate(c.indexAt)
	outgoing := model.CreateWsMessage(TypeUpdate)
	outgoing.Update = update
	message, err := json.Marshal(outgoing)
	if err == nil {
		c.indexAt++
		if c.indexAt >= model.Data.DataPointCount {
			c.indexAt = 0
		}
	} else {
		log.Println("Failed to marshal Update WebSocket message: %v", err)
	}
	return message
}
