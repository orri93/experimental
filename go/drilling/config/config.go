package config

type Configuration struct {
	ApiBinding       *Binding `yaml:"api-binding"`
	WebSocketBinding *Binding `yaml:"web-socket-binding"`
}

type Binding struct {
	Address string `yaml:"address"`
	Port    uint16 `yaml:"port"`
}
