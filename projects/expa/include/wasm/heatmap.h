#ifndef GOS_EXPA_WASM_HEATMAP_H_
#define GOS_EXPA_WASM_HEATMAP_H_

#include <stdbool.h>

#include <wasm/sdltypes.h>
#include <wasm/performance.h>

#ifdef __cplusplus
extern "C" {
#endif

extern gos_expa_data _expa_data;
extern gos_performance _performance;
extern gos_performance _interval_performance;

void gos_heatmap_create_random_vector(gos_vector* vector, double f);
bool gos_heatmap_initialize_performance();
bool gos_heatmap_initialize(int width, int height);
void gos_heatmap_draw();
void gos_heatmap_step();
void gos_heatmap_line();
void gos_heatmap_shutdown();

#ifdef __cplusplus
}
#endif

#endif
