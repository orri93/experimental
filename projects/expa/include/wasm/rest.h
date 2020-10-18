#ifndef GOS_EXPA_WASM_REST_H_
#define GOS_EXPA_WASM_REST_H_

/* https://emscripten.org/docs/api_reference/fetch.html */
/* https://magnealvheim.wordpress.com/2017/12/31/a-simple-webassembly-experiment/ */
#include <emscripten/fetch.h>

#ifdef __cplusplus
extern "C" {
#endif

void gos_rest_succeeded(emscripten_fetch_t* fetch);
void gos_rest_failed(emscripten_fetch_t* fetch);

#ifdef __cplusplus
}
#endif

#endif
