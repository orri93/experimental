#ifndef GOS_EXPA_WASM_DEMO_H_
#define GOS_EXPA_WASM_DEMO_H_

#include <stdbool.h>

#include <wasm/sdltypes.h>

#ifdef __cplusplus
extern "C" {
#endif

bool gos_heatmap_demo(gos_expa_data* expad, int width, int height);

#ifdef __cplusplus
}
#endif

#endif
