#ifndef GOS_EXPA_WASM_SDL_TYPES_H_
#define GOS_EXPA_WASM_SDL_TYPES_H_

#include <SDL.h>

#include <wasm/types.h>

typedef struct gos_expa_data {
  SDL_Surface* surface;
  gos_range_2d ranges;
  gos_rgb_gradient gradient;
  gos_expa_ws ws;
} gos_expa_data;

#endif
