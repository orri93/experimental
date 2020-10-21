#ifndef GOS_EXPA_WASM_UPDATE_H_
#define GOS_EXPA_WASM_UPDATE_H_

#include <stdbool.h>

#include <SDL.h>

#include <cJSON.h>

#include <wasm/types.h>

#ifdef __cplusplus
extern "C" {
#endif

bool gos_update_last_column_from_vector(
  gos_expa_data* expad,
  gos_point_2d_vector* v);

bool gos_update_column_from_vector(
  gos_expa_data* expad,
  gos_point_2d_vector* v,
  int i);

bool gos_update_from_json(gos_expa_data* expad, cJSON* m);

#ifdef __cplusplus
}
#endif

#endif
