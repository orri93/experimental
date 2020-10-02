#ifndef GOS_WASM_EXPERIMENTAL_H_
#define GOS_WASM_EXPERIMENTAL_H_

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

Uint32 gos_experimental_cubec(SDL_PixelFormat* format, int x, int y);
Uint32 gos_experimental_cubeca(SDL_PixelFormat* format, int x, int y, int a);

void gos_experimental_cube(SDL_Surface* surface);

#ifdef __cplusplus
}
#endif

#endif
