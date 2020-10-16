#include <assert.h>
#include <string.h>

#include <wasm/draw.h>
#include <gos/data.h>

static int _sdl_init_result = 1;

gos_rgb* gos_draw_gradient_intensity_rgb(
  gos_rgb_gradient* gradient,
  double intensity) {
  int i = intensity >= 0.0 ?
    (intensity <= 1.0 ? ((int)(((double)(gradient->count - 1)) * intensity)) :
      gradient->count - 1) : 0;
  return &(gradient->gradient[i]);
}

void gos_draw_setpixel(
  SDL_Surface* surface,
  int x,
  int y,
  int width,
  Uint32 pixel) {
  *(((Uint32*)(surface->pixels)) + x + ((size_t)y) * ((size_t)width)) = pixel;
}

void gos_draw_gradient_setpixel(
  SDL_Surface* surface,
  gos_rgb_gradient* gradient,
  int x,
  int y,
  int width,
  double intensity) {
  Uint32 pixel;
  gos_rgb* rgb = gos_draw_gradient_intensity_rgb(gradient, intensity);
  pixel = gos_draw_to_pixel(surface->format, rgb);
  gos_draw_setpixel(surface, x, y, width, pixel);
}

bool gos_draw_initialize(SDL_Surface** surface, int width, int height) {
  if ((_sdl_init_result = SDL_Init(SDL_INIT_VIDEO)) == 0) {
    *surface = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

    /* Enable Unicode translation */
    SDL_EnableUNICODE(1);

    return *surface != NULL;
  } else {
    return false;
  }
}

void gos_draw_shift_d1d1(SDL_Surface* surface) {
  int i;
  Uint32* pixels = (Uint32*)(surface->pixels);
  for (i = 0; i < surface->h; i++) {
    memmove(pixels, pixels + 1, (((size_t)(surface->w)) - 1) * sizeof(Uint32));
    pixels += surface->w;
  }
}

Uint32 gos_draw_to_pixel(SDL_PixelFormat* format, gos_rgb* rgb) {
  assert(format != NULL);
  assert(rgb != NULL);
  return SDL_MapRGB(format, rgb->r, rgb->g, rgb->b);
}

void gos_draw_vector_d1(
  SDL_Surface* surface,
  gos_vector* vector,
  gos_rgb_gradient* gradient,
  int index) {
  int i, c;
  Uint32 pixel;
  gos_rgb* rgb;
  for (i = 0; i < vector->count; i++) {
    rgb = gos_draw_gradient_intensity_rgb(gradient, vector->data[i]);
    pixel = gos_draw_to_pixel(surface->format, rgb);
    gos_draw_setpixel(surface, index, i, surface->w, pixel);
  }
}

void gos_draw_matrix(
  SDL_Surface* surface,
  gos_matrix* matrix,
  gos_rgb_gradient* gradient) {
  int i, j, c;
  Uint32 pixel;
  assert(surface != NULL);
  assert(matrix != NULL);
  assert(gradient != NULL);
  for (i = 0; i < matrix->countd1; i++) {
    for (j = 0; j < matrix->countd2; j++) {
      c = (int)(((double)(gradient->count - 1)) * gos_data_get_matrix(matrix, i, j));
      assert(c < gradient->count);
      pixel = gos_draw_to_pixel(surface->format, gradient->gradient + c);
      gos_draw_setpixel(surface, i, j, matrix->countd1, pixel);
    }
  }
}

void gos_draw_lock(SDL_Surface* surface) {
  if (SDL_MUSTLOCK(surface)) {
    SDL_LockSurface(surface);
  }
}
void gos_draw_unlock(SDL_Surface* surface) {
  if (SDL_MUSTLOCK(surface)) {
    SDL_UnlockSurface(surface);
  }
}

void gos_draw_shutdown() {
  if (_sdl_init_result == 0) {
    SDL_Quit();
    _sdl_init_result = 1;
  }
}
