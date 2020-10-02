#include <gos/draw.h>
#include <gos/experimental.h>

Uint32 gos_experimental_cubec(SDL_PixelFormat* format, int x, int y) {
  return gos_experimental_cubeca(format, x, y, (x + y) % 255);
}

Uint32 gos_experimental_cubeca(SDL_PixelFormat* format, int x, int y, int a) {
  return SDL_MapRGBA(format, x, y, 255 - x, a);
}

void gos_experimental_cube(SDL_Surface* surface) {
  int i, j;
  Uint32 pixel;
  for (i = 0; i < 256; i++) {
    for (j = 0; j < 256; j++) {
      pixel = gos_experimental_cubec(surface->format, i, j);
      gos_draw_setpixel(surface, i, j, 256, pixel);
    }
  }
}
