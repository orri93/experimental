#ifndef GOS_EXPA_WASM_VECTOR_H_
#define GOS_EXPA_WASM_VECTOR_H_

#include <stdbool.h>

#include <gos/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int gos_vector_index(gos_point_2d_vector* vector, int row);

bool gos_vector_get_points(
  gos_point_2d_vector* v,
  gos_point_2d* p1,
  gos_point_2d* p2,
  int i);

#ifdef __cplusplus
}
#endif

#endif
