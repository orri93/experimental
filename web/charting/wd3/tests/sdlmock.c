#include <stdlib.h>

#include <SDL.h>

#define SDL_MOCK_RESULT_SUCCESS 0

int SDL_Init(Uint32 flags) {
  return SDL_MOCK_RESULT_SUCCESS;
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, Uint32 flags) {
  return NULL;
}

Uint32 SDL_MapRGB(
  const SDL_PixelFormat* const format,
  const Uint8 r,
  const Uint8 g,
  const Uint8 b) {
  return SDL_MOCK_RESULT_SUCCESS;
}

int SDL_LockSurface(SDL_Surface* surface) {
  return SDL_MOCK_RESULT_SUCCESS;
}

void SDL_UnlockSurface(SDL_Surface* surface) {
  /* Do nothing */
}

int SDL_Flip(SDL_Surface* screen) {
  return SDL_MOCK_RESULT_SUCCESS;
}

int SDL_PollEvent(SDL_Event* event) {
  return SDL_MOCK_RESULT_SUCCESS;
}

char* SDL_GetError(void) {
  return "";
}

void SDL_Quit(void) {
  /* Do nothing */
}
