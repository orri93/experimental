#include <stdio.h>

#include <wasm/ws.h>

EM_BOOL gos_ws_on_open(int t, const EmscriptenWebSocketOpenEvent* e, void* d) {
  printf("WebSocket Open with event type: %d\n", t);
  return EM_TRUE;
}

EM_BOOL gos_ws_on_error(int t, const EmscriptenWebSocketOpenEvent* e, void* d) {
  printf("WebSocket Error with event type: %d\n", t);
  return EM_TRUE;
}

EM_BOOL gos_ws_on_close(int t, const EmscriptenWebSocketOpenEvent* e, void* d) {
  printf("WebSocket Close with event type: %d\n", t);
  return EM_TRUE;
}

EM_BOOL gos_ws_on_message(int t, const EmscriptenWebSocketOpenEvent* e, void* d) {
  printf("WebSocket Message with event type: %d\n", t);
  return EM_TRUE;
}
