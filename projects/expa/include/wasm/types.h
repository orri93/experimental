#ifndef GOS_EXPA_WASM_TYPES_H_
#define GOS_EXPA_WASM_TYPES_H_

#include <SDL.h>

#include <gos/types.h>

typedef struct gos_expa_ws {
  int start_from;
} gos_expa_ws;

typedef struct gos_expa_data {
  SDL_Surface* surface;
  gos_rgb_gradient gradient;
  gos_expa_ws ws;
} gos_expa_data;

#endif
