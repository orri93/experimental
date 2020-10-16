package ws

import (
	"encoding/json"
	"log"
	"net/http"
	"strings"
	"time"

	"github.com/gorilla/websocket"

	"../../../configuration"
	"../../model"
)

const (
	// Time allowed to write a message to the peer.
	writeWait = 10 * time.Second

	// Time allowed to read the next pong message from the peer.
	pongWait = 60 * time.Second

	// Send pings to peer with this period. Must be less than pongWait.
	pingPeriod = (pongWait * 9) / 10

	// Maximum message size allowed from peer.
	maxMessageSize = 512
)

var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,
}

// Client is a middleman between the websocket connection and the hub.
type Client struct {
	hub *Hub

	// The Web Socket connection
	connection *websocket.Conn

	// Buffered channel of outbound messages.
	send chan []byte

	active bool

	indexAt uint32
}

// Process messages from a client
func (c *Client) readPump() {
	defer func() {
		c.hub.unregister <- c
		c.connection.Close()
	}()
	c.connection.SetReadLimit(maxMessageSize)
	c.connection.SetReadDeadline(time.Now().Add(pongWait))
	c.connection.SetPongHandler(func(string) error {
		c.connection.SetReadDeadline(time.Now().Add(pongWait))
		return nil
	})
	for {
		_, message, err := c.connection.ReadMessage()
		if err != nil {
			if websocket.IsUnexpectedCloseError(err, websocket.CloseGoingAway, websocket.CloseAbnormalClosure) {
				log.Printf("error: %v", err)
			}
			break
		}
		log.Println("WebSocket Message from web client: " + string(message))

		incoming := model.WsMessage{}
		err = json.Unmarshal(message, &incoming)
		if err == nil {
			response := []byte{}
			response = nil
			if strings.Compare(incoming.Type, TypeStart) == 0 {
				response = c.handleStartRequest(incoming.Start)
			} else if strings.Compare(incoming.Type, TypeStop) == 0 {
				response = c.handleStopRequest()
			}
			if response != nil {
				c.send <- response
			}
		} else {
			log.Printf("error: %v", err)
		}

		//message = bytes.TrimSpace(bytes.Replace(message, newline, space, -1))
		//c.hub.broadcast <- message
	}
}

func (c *Client) writePump() {
	updatePeriod := time.Duration(configuration.Instance.Hub.Interval) * time.Millisecond
	ticker := time.NewTicker(pingPeriod)
	updateTicker := time.NewTicker(updatePeriod)
	defer func() {
		ticker.Stop()
		c.connection.Close()
	}()
	for {
		select {
		case message, ok := <-c.send:
			c.connection.SetWriteDeadline(time.Now().Add(writeWait))
			if !ok {
				// The hub closed the channel.
				c.connection.WriteMessage(websocket.CloseMessage, []byte{})
				return
			}

			w, err := c.connection.NextWriter(websocket.TextMessage)
			if err != nil {
				return
			}
			w.Write(message)

			// Add queued chat messages to the current websocket message.
			n := len(c.send)
			for i := 0; i < n; i++ {
				//w.Write(newline)
				w.Write(<-c.send)
			}

			if err := w.Close(); err != nil {
				return
			}
		case <-ticker.C:
			c.connection.SetWriteDeadline(time.Now().Add(writeWait))
			if err := c.connection.WriteMessage(websocket.PingMessage, nil); err != nil {
				return
			}
		case <-updateTicker.C:
			if c.active {
				updateMessage := c.createUpdate()
				if updateMessage != nil {
					c.send <- updateMessage
				}
			}
		}
	}
}

// serveWs handles Web Socket requests from the peer.
func serveWs(hub *Hub, w http.ResponseWriter, r *http.Request) {
	con, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println(err)
		return
	}

	client := &Client{hub: hub, connection: con, send: make(chan []byte, 256)}
	client.hub.register <- client

	// Allow collection of memory referenced by the caller by doing all work in
	// new goroutines.
	go client.writePump()
	go client.readPump()
}

// GetWsService get a websocket
func GetWsService(w http.ResponseWriter, r *http.Request) {
	log.Println("Servicing a Web Socket for " + r.URL.Path)
	serveWs(&Instance, w, r)
}
