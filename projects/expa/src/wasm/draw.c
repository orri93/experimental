#include <assert.h>
#include <string.h>

#include <gos/data.h>

#include <wasm/draw.h>

static int _sdl_init_result = 1;

void gos_draw_get_pixel_pointer(SDL_Surface* surface, Uint32** p, int x, int y) {
  *p = (((Uint32*)(surface->pixels)) + x + ((size_t)y) * ((size_t)surface->w));
}

gos_rgb* gos_draw_gradient_intensity_rgb(gos_rgb_gradient* gradient, double i) {
  int n = i >= 0.0 ? (i <= 1.0 ? ((int)(((double)(gradient->count - 1)) * i)) :
    gradient->count - 1) : 0;
  return &(gradient->gradient[n]);
}

void gos_draw_setpixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
  Uint32* pointer;
  gos_draw_get_pixel_pointer(surface, &pointer, x, y);
  *pointer = pixel;
  //*(((Uint32*)(surface->pixels)) + x + ((size_t)y) * ((size_t)surface->w)) = pixel;
}

void gos_draw_gradient_setpixel(gos_expa_data* expad, int x, int y, double i) {
  Uint32 pixel;
  SDL_Surface* surface = expad->surface;
  gos_rgb* rgb = gos_draw_gradient_intensity_rgb(&(expad->gradient), i);
  pixel = gos_draw_to_pixel(surface->format, rgb);
  gos_draw_setpixel(surface, x, y, pixel);
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

void gos_draw_vector_d1(gos_expa_data* expad, gos_vector* vector, int index) {
  int i;
  Uint32 pixel;
  gos_rgb* rgb;
  SDL_Surface* surface = expad->surface;
  for (i = 0; i < vector->count; i++) {
    rgb = gos_draw_gradient_intensity_rgb(&(expad->gradient), vector->data[i]);
    pixel = gos_draw_to_pixel(surface->format, rgb);
    gos_draw_setpixel(surface, index, i, pixel);
  }
}

void gos_draw_matrix(gos_expa_data* expad, gos_matrix* matrix) {
  int i, j, c;
  Uint32 pixel;
  SDL_Surface* surface;
  assert(expad != NULL);
  surface = expad->surface;
  assert(surface != NULL);
  assert(matrix != NULL);
  for (i = 0; i < matrix->countd1; i++) {
    for (j = 0; j < matrix->countd2; j++) {
      c = (int)(((double)((expad->gradient).count - 1)) *
        gos_data_get_matrix(matrix, i, j));
      assert(c < (expad->gradient).count);
      pixel = gos_draw_to_pixel(surface->format, (expad->gradient).gradient + c);
      gos_draw_setpixel(surface, i, j, pixel);
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
