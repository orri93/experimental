/*
 * Compilation
 * emcc -Os src/app/sdl-cube/sdl-cube.c -o src/assets/wasm/sdl-cube.js -s LEGACY_GL_EMULATION=1 -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']" -s MODULARIZE=1 -s EXPORT_NAME="CubeSdlModule"
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <emscripten.h>

#include <SDL/SDL.h>

static Uint32 cubec(SDL_PixelFormat* format, int x, int y);
static Uint32 cubeca(SDL_PixelFormat* format, int x, int y, int a);
static int sdlex();

int main(int argc, char** argv) {
  return sdlex();
}

Uint32 cubec(SDL_PixelFormat* format, int x, int y) {
  return cubeca(format, x, y, (x + y) % 255);
}

Uint32 cubeca(SDL_PixelFormat* format, int x, int y, int a) {
  return SDL_MapRGBA(format, x, y, 255 - x, a);
}

int sdlex() {
  printf("hello, world!\n");

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen = SDL_SetVideoMode(256, 256, 32, SDL_SWSURFACE);

  if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      * ((Uint32*)screen->pixels + i * 256 + j) = cubec(screen->format, i, j);
    }
  }
  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  SDL_Flip(screen);

  printf("you should see a smoothly-colored square - no sharp lines but the square borders!\n");
  printf("and here is some text that should be HTML-friendly: amp: |&| double-quote: |\"| quote: |'| less-than, greater-than, html-like tags: |<cheez></cheez>|\nanother line.\n");

  SDL_Quit();

  return 0;
}
