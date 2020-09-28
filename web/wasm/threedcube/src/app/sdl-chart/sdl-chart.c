/*
 * Compilation
 * emcc -Os src/app/sdl-cube/sdl-chart.c -o src/assets/wasm/sdl-chart.js -s LEGACY_GL_EMULATION=1 -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']" -s MODULARIZE=1 -s EXPORT_NAME="ChartSdlModule"
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <emscripten.h>

#include <SDL/SDL.h>

static int randomn(int maximum);
static Uint32 randomc(SDL_PixelFormat* format);
static Uint32 cubec(SDL_PixelFormat* format, int x, int y);
static Uint32 cubeca(SDL_PixelFormat* format, int x, int y, int a);
static int sdlex(int width, int height);

int main(int argc, char** argv) {

  int width = 600;
  int height = 400;

  if(argc > 1) {
    width = atoi(argv[1]);
  }
  if(argc > 2) {
    height = atoi(argv[2]);
  }

  return sdlex(width, height);
}

int randomn(int maximum) {
  return (int)(((long)maximum * (long)rand()) / RAND_MAX);
}

Uint32 randomc(SDL_PixelFormat* format) {
  return SDL_MapRGBA(
    format,
    randomn(255),
    randomn(255),
    randomn(255),
    randomn(255));
}

Uint32 cubec(SDL_PixelFormat* format, int x, int y) {
  return cubeca(format, x, y, (x + y) % 255);
}

Uint32 cubeca(SDL_PixelFormat* format, int x, int y, int a) {
  return SDL_MapRGBA(format, x, y, 255 - x, a);
}

int sdlex(int width, int height) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

  if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
  for (int i = 0; i < 128; i++) {
    for (int j = 0; j < 256; j++) {
      *((Uint32*)screen->pixels + i * 600 + j) = cubec(screen->format, i, j);
    }
  }
  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  SDL_Flip(screen);

  SDL_Quit();

  return 0;
}
