package ws

import "log"

// Hub structure export
type Hub struct {
	// Registered clients.
	clients map[*Client]bool

	// Inbound messages from the clients.

	// Register requests from the clients.
	register chan *Client

	// Unregister requests from the clients.
	unregister chan *Client
}

// Instance export
var Instance Hub

// Initialize function export
func Initialize() {
	log.Println("Initialize the WebSocket Hub")
	//Instance.broadcast = make(chan []byte)
	Instance.register = make(chan *Client)
	Instance.unregister = make(chan *Client)
	Instance.clients = make(map[*Client]bool)
}

// Execute function export
func (h *Hub) Execute() {
	log.Println("Executing the WebSocket Hub")
	for {
		select {
		case client := <-h.register:
			h.clients[client] = true
		case client := <-h.unregister:
			if _, ok := h.clients[client]; ok {
				delete(h.clients, client)
				close(client.send)
			}
		}
	}
}
