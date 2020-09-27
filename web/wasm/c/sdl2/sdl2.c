/*
 *
 * Copyright 2020 Geirmundur Orri Sigurdsson. All right reserved.
 * 
 * Compile
 * emcc sdl2.c -o sdl2.js -s USE_SDL=2 -s LEGACY_GL_EMULATION=1
 *
 */

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

int sdl2ex() {
  printf("hello, world!\n");

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen = SDL_SetVideoMode(256, 256, 32, SDL_SWSURFACE);

  if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      // To emulate native behavior with blitting to screen, alpha component is ignored. Test that it is so by outputting
      // data (and testing that it does get discarded)
      int alpha = (i + j) % 255;
      * ((Uint32*)screen->pixels + i * 256 + j) = SDL_MapRGBA(screen->format, i, j, 255 - i, alpha);
    }
  }
  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  SDL_Flip(screen);

  printf("you should see a smoothly-colored square - no sharp lines but the square borders!\n");
  printf("and here is some text that should be HTML-friendly: amp: |&| double-quote: |\"| quote: |'| less-than, greater-than, html-like tags: |<cheez></cheez>|\nanother line.\n");

  SDL_Quit();

  return 0;
}

int main(int argc, char** argv) {
  return sdl2ex();
}
