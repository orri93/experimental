#ifndef GOS_EXPA_WASM_UPDATE_H_
#define GOS_EXPA_WASM_UPDATE_H_

#include <stdbool.h>

#include <SDL.h>

#include <cJSON.h>

#include <wasm/types.h>

#ifdef __cplusplus
extern "C" {
#endif

bool gos_update_column_from_vector(
  gos_expa_data* expad,
  gos_range_1d* xr,
  gos_range_1d* yr,
  gos_point_2d_vector* v,
  int i);

bool gos_update_column_from_json(
  gos_expa_data* expad,
  gos_range_1d* xr,
  gos_range_1d* yr,
  cJSON* e,
  int i);

bool gos_update_from_json(
  gos_expa_data* expad,
  gos_range_1d* xr,
  gos_range_1d* yr,
  cJSON* m);

#ifdef __cplusplus
}
#endif

#endif
