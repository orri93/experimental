#ifndef GOS_EXPA_WASM_WS_H_
#define GOS_EXPA_WASM_WS_H_

#include <stdbool.h>

#include <emscripten.h>
/* https://gist.github.com/nus/564e9e57e4c107faa1a45b8332c265b9 */
#include <emscripten/websocket.h>

#include <wasm/types.h>

#ifdef __cplusplus
extern "C" {
#endif

EM_BOOL gos_ws_on_open(int t, const EmscriptenWebSocketOpenEvent* e, void* d);
EM_BOOL gos_ws_on_error(int t, const EmscriptenWebSocketErrorEvent* e, void* d);
EM_BOOL gos_ws_on_close(int t, const EmscriptenWebSocketCloseEvent* e, void* d);
EM_BOOL gos_ws_on_message(int t, const EmscriptenWebSocketMessageEvent* e, void* d);

EMSCRIPTEN_RESULT gos_ws_send_start(gos_expa_ws* ws);
EMSCRIPTEN_RESULT gos_ws_send_stop(gos_expa_ws* ws);

EMSCRIPTEN_RESULT gos_ws_close(gos_expa_ws* ws, unsigned short c);
EMSCRIPTEN_RESULT gos_ws_delete(gos_expa_ws* ws);

EMSCRIPTEN_WEBSOCKET_T gos_ws_start(gos_expa_data* expad);


#ifdef __cplusplus
}
#endif

#endif
