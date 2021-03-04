# GOS WebAssembly D3 Heatmap Charting Experiment

This project was generated with [Angular CLI](https://github.com/angular/angular-cli) version 10.2.0.

## Development server

Run `ng serve` for a dev server. Navigate to `http://localhost:4200/`. The app will automatically reload if you change any of the source files.

## Code scaffolding

Run `ng generate component component-name` to generate a new component. You can also use `ng generate directive|pipe|service|class|guard|interface|enum|module`.

## Build

### C/C++

#### Visual Studio 2019

To build the SDL Desktop Targets set the SDL_DESKTOP_TARGET to True

Example
```json
    {
        "name": "SDL_DESKTOP_TARGET",
        "value": "True",
        "type": "BOOL"
    }
```

Set SDL_INCLUDE_DIR to the include folder, SDL_LIBRARY to the SDL.lib file and SDLMAIN_LIBRARY to the SDLmain.lib file of the SDL 1.2 library

Example
```json
    {
        "name": "SDL_INCLUDE_DIR",
        "value": "C:\\src\\SDL-1.2.15\\include",
        "type": "PATH"
    },
    {
        "name": "SDL_LIBRARY",
        "value": "C:\\src\\SDL-1.2.15\\VisualC\\x64\\Debug\\SDL.lib",
        "type": "FILEPATH"
    },
    {
        "name": "SDLMAIN_LIBRARY",
        "value": "C:\\src\\SDL-1.2.15\\VisualC\\SDLmain\\x64\\Debug\\SDLmain.lib",
        "type": "FILEPATH"
    }
```

### Angular

Run `ng build` to build the project. The build artifacts will be stored in the `dist/` directory. Use the `--prod` flag for a production build.

## Running SDL Desktop Targets

### In Visual Studio 2019

set the PATH environmental variable to the directory containing the SDL.dll file of the SDL 1.2 library in the launch.vs.json file.

Example
```json
{
  "version": "0.2.1",
  "defaults": {},
  "configurations": [
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "demo.exe (src\\modules\\demo\\Debug\\demo.exe)",
      "name": "demo.exe (src\\modules\\demo\\Debug\\demo.exe)",
      "env": {
        "PATH": "C:\\src\\SDL-1.2.15\\VisualC\\SDL\\Debug"
      }
    }
  ]
}
```

## Running unit tests

Run `ng test` to execute the unit tests via [Karma](https://karma-runner.github.io).

## Running end-to-end tests

Run `ng e2e` to execute the end-to-end tests via [Protractor](http://www.protractortest.org/).

## Further help

To get more help on the Angular CLI use `ng help` or go check out the [Angular CLI Overview and Command Reference](https://angular.io/cli) page.
