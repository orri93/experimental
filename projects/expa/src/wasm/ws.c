#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <cJSON.h>

#include <wasm/ws.h>
#include <wasm/json.h>
#include <wasm/types.h>

EM_BOOL gos_ws_on_open(int t, const EmscriptenWebSocketOpenEvent* e, void* d) {
  EMSCRIPTEN_RESULT result;
  gos_expa_data* expad;
  printf("WebSocket Open with event type: %d\n", t);
  expad = (gos_expa_data*)d;
  assert(expad != NULL);

  result = gos_ws_start(e->socket, expad->ws.start_from);
  if (result == EMSCRIPTEN_RESULT_SUCCESS) {
    fprintf(stderr, "Sending Start Web Socket message failed: %d\n", result);
  }

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

EMSCRIPTEN_RESULT gos_ws_send_json(EMSCRIPTEN_WEBSOCKET_T s, cJSON* message) {
  EMSCRIPTEN_RESULT result = EMSCRIPTEN_RESULT_FAILED;
  char* string;
  if (s != 0 && message != NULL) {
    string = cJSON_PrintUnformatted(message);
    if (string != NULL) {
      result = emscripten_websocket_send_utf8_text(s, string);
      free(string);
    }
  }
  if (message != NULL) {
    cJSON_Delete(message);
  }
  return result;
}

EMSCRIPTEN_RESULT gos_ws_start(EMSCRIPTEN_WEBSOCKET_T s, int from) {
  cJSON* message = NULL;
  if (gos_json_create_start_message(&message, from)) {
    return gos_ws_send_json(s, message);
  } else {
    return EMSCRIPTEN_RESULT_FAILED;
  }
}

EMSCRIPTEN_RESULT gos_ws_stop(EMSCRIPTEN_WEBSOCKET_T s) {
  cJSON* message;
  if (gos_json_create_message(&message, GosWsMsgStop)) {
    return gos_ws_send_json(s, message);
  } else {
    return EMSCRIPTEN_RESULT_FAILED;
  }
}

EMSCRIPTEN_RESULT gos_ws_close(EMSCRIPTEN_WEBSOCKET_T s, unsigned short c) {
  EMSCRIPTEN_RESULT result = EMSCRIPTEN_RESULT_FAILED;
  if (s != 0) {
    result = emscripten_websocket_close(s, c, "no reason");
  }
  return result;
}

EMSCRIPTEN_RESULT gos_ws_delete(EMSCRIPTEN_WEBSOCKET_T* s) {
  EMSCRIPTEN_RESULT result = EMSCRIPTEN_RESULT_FAILED;
  if (*s != 0) {
    result = emscripten_websocket_delete(*s);
    if (result == EMSCRIPTEN_RESULT_SUCCESS) {
      *s = 0;
    }
  }
  return result;
}
