package application

import (
	"log"
	"net/http"

	"github.com/gorilla/csrf"

	"../configuration"
	"./handler"
	"./handler/rest"
	"./handler/web"
	"./handler/ws"
)

// Application has handler
type Application struct {
	Handler handler.Handler
}

// Initialize initializes the application with predefined configuration
func (a *Application) Initialize(webpath string) {
	a.Handler.Initialize()
	a.setRouters(webpath)
}

// Execute the application on it's router
func (a *Application) Execute() {
	host := configuration.Instance.GetServiceHost()
	welcome := "Starting to service " + host + " with root path as " + configuration.Instance.Service.Path
	log.Println(welcome)

	crossKeyLength := len(configuration.Instance.Service.Cross.Key)

	if crossKeyLength > 0 {
		if crossKeyLength == 32 {
			log.Println("Using Cross-site Request Authentication Key")
			if len(configuration.Instance.Service.Cross.Trusted) > 0 {
				for _, origin := range configuration.Instance.Service.Cross.Trusted {
					log.Println("Adding '" + origin + "' as trusted origin")
				}
				trusted := csrf.TrustedOrigins(configuration.Instance.Service.Cross.Trusted)
				protection := csrf.Protect([]byte("32-byte-long-auth-key"), trusted)
				log.Fatal(http.ListenAndServe(host, protection(a.Handler.Router)))
			} else {
				protection := csrf.Protect([]byte("32-byte-long-auth-key"), csrf.SameSite(csrf.SameSiteNoneMode))
				log.Fatal(http.ListenAndServe(host, protection(a.Handler.Router)))
			}
		} else {
			log.Fatal("The Cross-site Request Authentication Key is not 32 byte long")
		}
	} else {
		log.Fatal(http.ListenAndServe(host, a.Handler.Router))
	}
}

// setRouters sets the all required routers
func (a *Application) setRouters(webpath string) {
	if len(webpath) > 0 {
		path := configuration.Instance.Service.Path
		web := web.NewWeb(path, webpath, "index.html", `^.+\.(ico|js|html|css)$`)
		assets := web.AddChildFolder("assets/", `^.+\.(json)$`)
		assets.AddChildFolder("wasm/", `^.+\.(js|wasm)$`)
		a.Handler.Web(web)
	}
	a.Handler.Get("data/matrix/{count}", rest.HandleMatrixRequest)
	a.Handler.WebSocket("ws", ws.GetWsService)
}
