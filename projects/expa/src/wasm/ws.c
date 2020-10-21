#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <cJSON.h>

#include <wasm/ws.h>
#include <wasm/json.h>
#include <wasm/draw.h>
#include <wasm/types.h>
#include <wasm/update.h>
#include <wasm/heatmap.h>

static EMSCRIPTEN_RESULT gos_ws_send_json(
  EMSCRIPTEN_WEBSOCKET_T s,
  cJSON* message);

EM_BOOL gos_ws_on_open(int t, const EmscriptenWebSocketOpenEvent* e, void* d) {
  EMSCRIPTEN_RESULT result;
  gos_expa_data* expad;
  printf("WebSocket Open with event type: %d\n", t);
  expad = (gos_expa_data*)d;
  assert(expad != NULL);

  (expad->ws).socket = e->socket;

  if ((expad->ws).start_from > 0) {
    result = gos_ws_send_start(&(expad->ws));
    if (result != EMSCRIPTEN_RESULT_SUCCESS) {
      fprintf(stderr, "Sending Start Web Socket message failed: %d\n", result);
    }
  }

  return EM_TRUE;
}

EM_BOOL gos_ws_on_error(int t, const EmscriptenWebSocketErrorEvent* e, void* d) {
  printf("WebSocket Error with event type: %d\n", t);
  return EM_TRUE;
}

EM_BOOL gos_ws_on_close(int t, const EmscriptenWebSocketCloseEvent* e, void* d) {
  printf("WebSocket Close with event type: %d\n", t);
  return EM_TRUE;
}

EM_BOOL gos_ws_on_message(int t, const EmscriptenWebSocketMessageEvent* e, void* d) {
  cJSON* message;
  GosWsMessageType type;
  gos_json_ws_update update;
  gos_expa_data* expad;

  expad = (gos_expa_data*)d;
  assert(expad != NULL);

  if (e->isText) {
    if (e->numBytes > 0) {
      message = cJSON_ParseWithLength(
        (const char*)(e->data),
        (size_t)(e->numBytes));
      if (message != NULL) {
        type = gos_json_get_message_type(message);
        switch (type) {
        case GosWsMsgUpdate:
          if (gos_json_get_update(&update, message)) {
            gos_draw_lock(expad->surface);
            gos_draw_shift_d1d1(expad->surface);

            gos_update_last_column_from_vector(expad, &(update.v));

            gos_draw_unlock(expad->surface);
            SDL_Flip(expad->surface);
          }
          break;
        default:
          break;
        }
        cJSON_Delete(message);
      }
    }
  }

  return EM_TRUE;
}

EMSCRIPTEN_RESULT gos_ws_send_start(gos_expa_ws* ws) {
  cJSON* message = NULL;
  if (gos_json_create_start_message(&message, ws->start_from)) {
    return gos_ws_send_json(ws->socket, message);
  } else {
    return EMSCRIPTEN_RESULT_FAILED;
  }
}

EMSCRIPTEN_RESULT gos_ws_send_stop(gos_expa_ws* ws) {
  cJSON* message;
  if (gos_json_create_message(&message, GosWsMsgStop)) {
    return gos_ws_send_json(ws->socket, message);
  } else {
    return EMSCRIPTEN_RESULT_FAILED;
  }
}

EMSCRIPTEN_RESULT gos_ws_close(gos_expa_ws* ws, unsigned short c) {
  EMSCRIPTEN_RESULT result = EMSCRIPTEN_RESULT_FAILED;
  if (ws->socket != 0) {
    result = emscripten_websocket_close(ws->socket, c, "no reason");
  }
  return result;
}

EMSCRIPTEN_RESULT gos_ws_delete(gos_expa_ws* ws) {
  EMSCRIPTEN_RESULT result = EMSCRIPTEN_RESULT_FAILED;
  if (ws->socket != 0) {
    result = emscripten_websocket_delete(ws->socket);
    if (result == EMSCRIPTEN_RESULT_SUCCESS) {
      ws->socket = 0;
    }
  }
  return result;
}

EMSCRIPTEN_WEBSOCKET_T gos_ws_start(gos_expa_data* expad) {
  EMSCRIPTEN_WEBSOCKET_T ws = 0;
  EmscriptenWebSocketCreateAttributes attr;
  attr.url = (expad->ws).url;
  attr.protocols = NULL;
  attr.createOnMainThread = EM_TRUE;
  ws = emscripten_websocket_new(&attr);
  if (ws > 0) {
    emscripten_websocket_set_onopen_callback(ws, expad, gos_ws_on_open);
    emscripten_websocket_set_onerror_callback(ws, expad, gos_ws_on_error);
    emscripten_websocket_set_onclose_callback(ws, expad, gos_ws_on_close);
    emscripten_websocket_set_onmessage_callback(ws, expad, gos_ws_on_message);
  }
  return ws;
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
