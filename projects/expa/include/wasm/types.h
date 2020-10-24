#ifndef GOS_EXPA_WASM_TYPES_H_
#define GOS_EXPA_WASM_TYPES_H_

#include <SDL.h>

#include <gos/types.h>

#define GOS_EXPA_WASM_WS_URL 1024

typedef struct gos_expa_ws {
  int socket;
  int start_from;
  char url[GOS_EXPA_WASM_WS_URL];
} gos_expa_ws;

typedef struct gos_expa_data {
  SDL_Surface* surface;
  gos_range_2d ranges;
  gos_rgb_gradient gradient;
  gos_expa_ws ws;
} gos_expa_data;

#endif
