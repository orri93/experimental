/*
 *
 * Copyright 2020 Geirmundur Orri Sigurdsson. All right reserved.
 *
 */

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define SDL_CHART_WIDTH 1024
#define SDL_CHART_HEIGHT 640
#define SDL_CHART_BPP 32
#define SDL_CHART_FPS 60

#ifdef __cplusplus
extern "C" {
#endif

static bool issdlinited = false;
static bool issurfacelocked = false;

static SDL_Window* window = NULL;
static SDL_Surface* surface = NULL;
static SDL_Renderer* renderer = NULL;

static bool initialize();

static bool lock();
static void unlock();
static bool flip();

static void render();

static bool chessboard(SDL_Renderer * renderer);

static void shutdown();

static int map(int x, int min, int max, int mino, int maxo);

int main(int argc, char** argv) {
  int retresult = EXIT_SUCCESS;
  if(initialize()) {
#ifdef EMSCRIPTEN
    emscripten_set_main_loop(render, SDL_CHART_FPS, 1);
#else
    render();
#endif
  } else {
    retresult = EXIT_FAILURE;
  }
  shutdown();
  return retresult;
}

bool initialize() {
  int sdlresult;
  sdlresult = SDL_Init(SDL_INIT_VIDEO);
  if(sdlresult == 0) {
    surface = SDL_SetVideoMode(
      SDL_CHART_WIDTH,
      SDL_CHART_HEIGHT,
      SDL_CHART_BPP,
      SDL_SWSURFACE);
    if(surface != NULL) {
      renderer = SDL_CreateSoftwareRenderer(surface);
      if(renderer != NULL) {
        issdlinited = true;
        return true;
      } else {
        fprintf(
          stderr,
          "Failed to create SDL software renderer from surface: %s\n",
          SDL_GetError());
      }
    } else {
      fprintf(
        stderr,
        "Failed to get SDL surface: %s\n",
        SDL_GetError());
      return false;
    }
  } else {
    fprintf(
      stderr,
      "Failed to initialize SDL video with result code %d\n",
      sdlresult);
  }
  return false;
}

bool lock() {
  int sdlresult;
  assert(surface != NULL);
  if(SDL_MUSTLOCK(screen)) {
    sdlresult = SDL_LockSurface(surface);
    if(sdlresult == 0) {
      issurfacelocked = true;
      return true;
    } else {
      fprintf(
        stderr,
        "Failed to locking surface failed with result code %d\n",
        sdlresult);
      return false;
    }
  }
  return true;
}

void unlock() {
  assert(surface != NULL);
  if(issurfacelocked) {
    SDL_UnlockSurface(surface);
    issurfacelocked = false;
  }
}

bool flip() {
  int sdlresult;
  assert(surface != NULL);
  sdlresult = SDL_Flip(surface);
  if(sdlresult == 0) {
    return true;
  } else {
    fprintf(
      stderr,
      "Failed to flip the surface with result code %d\n",
      sdlresult);
    return false;
  }
}

void render() {

  assert(surface != NULL);
  assert(renderer != NULL);

  if(lock()) {

    /* Clear the rendering surface with the specified color */
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);

    chessboard(renderer);

    unlock();
    flip();
  }

}

bool chessboard(SDL_Renderer * renderer) {
  int row, column, x;
  SDL_Rect rect, darea;

  assert(renderer != NULL);

  /* Get the Size of drawing surface */
  SDL_RenderGetViewport(renderer, &darea);

  for(row = 0; row < 8; row++) {
    x = row % 2;
    for(column = x; column < 4 + (row % 2); column++) {
      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

      rect.w = darea.w / 8;
      rect.h = darea.h / 8;
      rect.x = x * rect.w;
      rect.y = row * rect.h;

      x += 2;

      SDL_RenderFillRect(renderer, &rect);
    }
  }
}

void shutdown() {
  if(renderer != NULL) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }
  if(issdlinited) {
    SDL_Quit();
    issdlinited = false;
  }
}

int map(int x, int min, int max, int mino, int maxo) {
  return mino + (((x - min) * (maxo - mino)) / (max - min));
}

#ifdef __cplusplus
}
#endif
