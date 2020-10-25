#ifndef GOS_EXPA_WASM_DRAW_H_
#define GOS_EXPA_WASM_DRAW_H_

#include <stdbool.h>

#include <SDL.h>

#include <wasm/sdltypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void gos_draw_get_pixel_pointer(SDL_Surface* surface, Uint32** p, int x, int y);

gos_rgb* gos_draw_gradient_intensity_rgb(gos_rgb_gradient* gradient, double i);

void gos_draw_setpixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

void gos_draw_gradient_setpixel(gos_expa_data* expad, int x, int y, double i);

bool gos_draw_initialize(SDL_Surface** surface, int width, int height);

void gos_draw_shift_d1d1(SDL_Surface* surface);

Uint32 gos_draw_to_pixel(SDL_PixelFormat* format, gos_rgb* rgb);

void gos_draw_vector_d1(gos_expa_data* expad, gos_vector* vector, int index);
void gos_draw_matrix(gos_expa_data* expad, gos_matrix* matrix);

void gos_draw_lock(SDL_Surface* surface);
void gos_draw_unlock(SDL_Surface* surface);

void gos_draw_shutdown();

#ifdef __cplusplus
}
#endif

#endif
