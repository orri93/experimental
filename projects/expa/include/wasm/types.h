#ifndef GOS_EXPA_WASM_TYPES_H_
#define GOS_EXPA_WASM_TYPES_H_

#include <SDL.h>

/* https://gist.github.com/nus/564e9e57e4c107faa1a45b8332c265b9 */
#include <emscripten/websocket.h>

#include <gos/types.h>

#define GOS_EXPA_WASM_WS_URL 1024

typedef struct gos_expa_ws {
  EMSCRIPTEN_WEBSOCKET_T socket;
  int start_from;
  char url[GOS_EXPA_WASM_WS_URL];
} gos_expa_ws;

typedef struct gos_expa_ranges {
  gos_range_1d x;
  gos_range_1d y;
} gos_expa_ranges;

typedef struct gos_expa_data {
  SDL_Surface* surface;
  gos_expa_ranges ranges;
  gos_rgb_gradient gradient;
  gos_expa_ws ws;
} gos_expa_data;

#endif
