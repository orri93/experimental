#include <assert.h>

#include <gos/draw.h>
#include <gos/data.h>

static int _sdl_init_result = 1;

void gos_draw_setpixel(
  SDL_Surface* surface,
  int x,
  int y,
  int width,
  Uint32 pixel) {
  *(((Uint32*)(surface->pixels)) + x + ((size_t)y) * ((size_t)width)) = pixel;
}

bool gos_draw_initialize(SDL_Surface** surface, int width, int height) {
  if ((_sdl_init_result = SDL_Init(SDL_INIT_VIDEO)) == 0) {
    *surface = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
    return *surface != NULL;
  } else {
    return false;
  }
}

Uint32 gos_draw_to_pixel(SDL_PixelFormat* format, gos_rgb* rgb) {
  assert(format != NULL);
  assert(rgb != NULL);
  return SDL_MapRGB(format, rgb->r, rgb->g, rgb->b);
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
