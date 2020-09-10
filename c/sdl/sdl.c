#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <gos/experimental/sdl/render.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

static bool sdlinited = false;

/* The window we'll be rendering to */
static SDL_Window* window = NULL;

/* The surface contained by the window */
static SDL_Surface* surface = NULL;

int main(int argc, char** argv) {
  int mainres = EXIT_SUCCESS;

  /* Initialize SDL */
  if ((sdlinited = SDL_Init(SDL_INIT_VIDEO) == 0)) {
    /* Create window */
    window = SDL_CreateWindow(
      "SDL Tutorial",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_SHOWN);
    if (window) {
      
      /* Get window surface */
      surface = SDL_GetWindowSurface(window);

      /* Fill the surface white */
      SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

      /* Update the surface */
      SDL_UpdateWindowSurface(window);

      /* Wait two seconds */
      SDL_Delay(2000);

    } else {
      fprintf(
        stderr,
        "Window could not be created! SDL_Error: %s\n",
        SDL_GetError());
      mainres = EXIT_FAILURE;
    }
  } else {
    fprintf(
      stderr,
      "SDL could not initialize! SDL_Error: %s\n",
      SDL_GetError());
    mainres = EXIT_FAILURE;
  }

  if (window != NULL) {
    SDL_DestroyWindow(window);
    window = NULL;
  }

  /* Quit SDL subsystems */
  SDL_Quit();

  return mainres;
}
