#ifndef GOS_WASM_DRAW_H_
#define GOS_WASM_DRAW_H_

#include <stdbool.h>

#include <SDL.h>

#include <gos/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void gos_draw_setpixel(
  SDL_Surface* surface,
  int x,
  int y,
  int width,
  Uint32 pixel);

bool gos_draw_initialize(SDL_Surface** surface, int width, int height);

void gos_draw_shift_d1d1(SDL_Surface* surface);

Uint32 gos_draw_to_pixel(SDL_PixelFormat* format, gos_rgb* rgb);

void gos_draw_vector_d1(
  SDL_Surface* surface,
  gos_vector* vector,
  gos_rgb_gradient* gradient,
  int inxed);
void gos_draw_matrix(
  SDL_Surface* surface,
  gos_matrix* matrix,
  gos_rgb_gradient* gradient);

void gos_draw_lock(SDL_Surface* surface);
void gos_draw_unlock(SDL_Surface* surface);

void gos_draw_shutdown();

#ifdef __cplusplus
}
#endif

#endif
