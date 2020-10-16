#ifndef GOS_EXPA_WASM_CHART_H_
#define GOS_EXPA_WASM_CHART_H_

#include <stdbool.h>

#include <gos/types.h>
#include <gos/color.h>

#ifdef __cplusplus
extern "C" {
#endif

int gos_chart_vector_index(gos_point_2d_vector* vector, int row);

bool gos_chart_create_gradient(gos_rgb_gradient* gradient);

bool gos_wasm_chart_initialize(int width, int height);

#ifdef __cplusplus
}
#endif

#endif
