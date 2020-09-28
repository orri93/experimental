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
static Uint32 cubec(SDL_PixelFormat* format, int x, int y);
static Uint32 cubeca(SDL_PixelFormat* format, int x, int y, int a);
static void setpixel(SDL_Surface* surface, int x, int y, int width, Uint32 pixel);
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

Uint32 cubec(SDL_PixelFormat* format, int x, int y) {
  return cubeca(format, x, y, (x + y) % 255);
}

Uint32 cubeca(SDL_PixelFormat* format, int x, int y, int a) {
  return SDL_MapRGBA(format, x, y, 255 - x, a);
}

void setpixel(SDL_Surface* surface, int x, int y, int width, Uint32 pixel) {
  *(((Uint32*)(surface->pixels)) + x + y * width) = pixel;
}

int sdlex(int width, int height) {
  int r, g, b, n, m;
  Uint32 pixel;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

  n = m = 0;

  if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
  /* i is height and j is width */
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      //r = randomn(255);
      //g = randomn(255);
      //b = randomn(255);
      //pixel = SDL_MapRGB(screen->format, r, g, b);
      pixel = cubec(screen->format, n, m);
      setpixel(screen, j, i, width, pixel);
      n++;
      if(n >= 256) {
        n = 0;
      }
    }
    m++;
    if(m >= 256) {
      m = 0;
    }
  }
  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  SDL_Flip(screen);

  SDL_Quit();

  return 0;
}
