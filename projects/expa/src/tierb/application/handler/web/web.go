package web

import (
	"log"
	"net/http"
	"os"
	"path"
	"path/filepath"
	"regexp"
	"strings"

	"github.com/gorilla/mux"
)

// Web structure export
type Web struct {
	Path      string
	Index     string
	Directory string
	Files     map[string]string
	Folders   map[string]*Web
}

// NewWeb Constructor export
func NewWeb(path string, dir string, index string, regX string) *Web {
	web := new(Web)
	web.Path = path
	web.Index = index
	web.Directory = dir
	web.Files = make(map[string]string)
	web.Folders = make(map[string]*Web)
	indexIncluded := false
	var rx = regexp.MustCompile(regX)
	log.Println("Adding '" + path + "' as Web directory for '" + dir + "'")
	err := filepath.Walk(dir, func(fPath string, info os.FileInfo, err error) error {
		if rx.MatchString(fPath) {
			d, f := filepath.Split(fPath)
			log.Println("Adding file '" + f + "' in directory '" + d + "' with file path '" + fPath)
			web.Files[f] = fPath
			if strings.Compare(index, f) == 0 {
				log.Println("Index file '" + index + "' found")
				indexIncluded = true
			}
		}
		return nil
	})
	if err != nil {
		log.Fatalf("Error walking directory '%s': %v", dir, err)
	}
	if !indexIncluded && len(index) > 0 {
		log.Fatalf("The index file '" + index + "' was not found in the directory")
	}
	return web
}

// AddFolder function export
func (web *Web) AddFolder(path string, dir string, index string, regX string) *Web {
	folder := NewWeb(path, dir, index, regX)
	web.Folders[path] = folder
	return folder
}

// AddChildFolder function export
func (web *Web) AddChildFolder(path string, regX string) *Web {
	childPath := web.Path + path
	directory := filepath.Join(web.Directory, path)
	return web.AddFolder(childPath, directory, "", regX)
}

// AddChildFolderWithIndex function export
func (web *Web) AddChildFolderWithIndex(path string, index string, regX string) *Web {
	childPath := web.Path + path
	directory := filepath.Join(web.Directory, path)
	return web.AddFolder(childPath, directory, index, regX)
}

// AddNamedChildFolder function export
func (web *Web) AddNamedChildFolder(path string, dir string, regX string) *Web {
	childPath := web.Path + path
	directory := filepath.Join(web.Directory, dir)
	return web.AddFolder(childPath, directory, "", regX)
}

// AddNamedChildFolderWithIndex function export
func (web *Web) AddNamedChildFolderWithIndex(path string, dir string, index string, regX string) *Web {
	childPath := web.Path + path
	directory := filepath.Join(web.Directory, dir)
	return web.AddFolder(childPath, directory, index, regX)
}

// CreateRoutes function export
func (web *Web) CreateRoutes(router *mux.Router) {
	router.Handle(web.Path, web)
	for k, e := range web.Files {
		path := web.Path + k
		log.Println("Adding route for '" + path + "' with file '" + e + "'")
		router.Handle(path, web)
	}
	for k, e := range web.Folders {
		log.Println("Adding route for child web '" + k + "'")
		e.CreateRoutes(router)
	}
}

func (web *Web) ServeHTTP(writer http.ResponseWriter, request *http.Request) {
	log.Println("Servicing a web page for " + request.URL.Path)
	if request.Method != "GET" {
		http.Error(writer, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}
	var file string
	if strings.Compare(request.URL.Path, web.Path) == 0 {
		file = web.Index
	} else {
		file = path.Base(request.URL.Path)
	}
	fp := web.Files[file]
	http.ServeFile(writer, request, fp)
}
