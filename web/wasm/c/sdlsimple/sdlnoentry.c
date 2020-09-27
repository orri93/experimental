/* 
 * No Entry
 * emcc -Os sdl.c -o sdlnoentry.wasm --no-entry
 * 
 */

#include <stdio.h>
#include <SDL/SDL.h>

#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int sdlex() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen = SDL_SetVideoMode(256, 256, 32, SDL_SWSURFACE);

  if (SDL_MUSTLOCK(screen)) {
    SDL_LockSurface(screen);
  }
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      // To emulate native behavior with blitting to screen, alpha component is ignored. Test that it is so by outputting
      // data (and testing that it does get discarded)
      int alpha = (i + j) % 255;
      *((Uint32*)screen->pixels + i * 256 + j) = 
        SDL_MapRGBA(screen->format, i, j, 255 - i, alpha);
    }
  }
  if (SDL_MUSTLOCK(screen)) {
    SDL_UnlockSurface(screen);
  }
  SDL_Flip(screen);

  SDL_Quit();

  return 0;
}

