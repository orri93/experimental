#ifndef GOS_EXPA_WASM_WS_H_
#define GOS_EXPA_WASM_WS_H_

#include <stdbool.h>

/* https://gist.github.com/nus/564e9e57e4c107faa1a45b8332c265b9 */
#include <emscripten/websocket.h>

#include <gos/types.h>

#ifdef __cplusplus
extern "C" {
#endif

EM_BOOL gos_ws_on_open(int t, const EmscriptenWebSocketOpenEvent* e, void* d);
EM_BOOL gos_ws_on_error(int t, const EmscriptenWebSocketOpenEvent* e, void* d);
EM_BOOL gos_ws_on_close(int t, const EmscriptenWebSocketOpenEvent* e, void* d);
EM_BOOL gos_ws_on_message(int t, const EmscriptenWebSocketOpenEvent* e, void* d);

#ifdef __cplusplus
}
#endif

#endif
