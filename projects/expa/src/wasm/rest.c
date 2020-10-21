#include <assert.h>
#include <stdio.h>

#include <wasm/ws.h>
#include <wasm/rest.h>
#include <wasm/json.h>
#include <wasm/types.h>
#include <wasm/update.h>

static void gos_rest_update(emscripten_fetch_t* fetch);
static void gos_rest_shutdown(emscripten_fetch_t* fetch);

void gos_rest_advanced_succeeded(emscripten_fetch_t* fetch) {
  gos_expa_data* expad;

  gos_rest_update(fetch);

  expad = (gos_expa_data*)(fetch->userData);
  assert(expad != NULL);

  if ((expad->ws).socket > 0) {
    gos_ws_send_start(&(expad->ws));
  } else {
    gos_ws_start(expad);
  }

  gos_rest_shutdown(fetch);
}

void gos_rest_succeeded(emscripten_fetch_t* fetch) {
  gos_rest_update(fetch);
  gos_rest_shutdown(fetch);
}

void gos_rest_failed(emscripten_fetch_t* fetch) {
  printf(
    "Downloading %s failed, HTTP failure status code: %d.\n",
    fetch->url,
    fetch->status);
  /* Also free data on failure */
  emscripten_fetch_close(fetch);
}

void gos_rest_update(emscripten_fetch_t* fetch) {
  gos_expa_data* expad;
  cJSON* v;
  cJSON* json;

  printf("Finished downloading %llu bytes from URL %s.\n",
    fetch->numBytes,
    fetch->url);

  expad = (gos_expa_data*)(fetch->userData);
  assert(expad != NULL);

  /*
   * The data is now available at
   *   fetch->data[0] through fetch->data[fetch->numBytes-1];
   */
  if (fetch->numBytes > 0) {
    json = cJSON_ParseWithLength(fetch->data, (size_t)(fetch->numBytes));
    if (json != NULL) {
      if (cJSON_IsObject(json)) {
        if (gos_json_get_ranges(&(expad->ranges), json)) {
          printf(
            "JSON X range is from %f to %f\n",
            (expad->ranges).x.from,
            (expad->ranges).x.to);
          printf(
            "JSON Y range is from %f to %f\n",
            (expad->ranges).y.from,
            (expad->ranges).y.to);
          if (!gos_update_from_json(expad, json)) {
            fprintf(stderr, "Failed to update from JSON\n");
          }
        } else {
          fprintf(stderr, "Failed to get ranges\n");
        }
      } else {
        fprintf(stderr, "JSON is not an object\n");
      }
      cJSON_Delete(json);
    } else {
      fprintf(stderr, "Failed to parse JSON\n");
    }
  }
}

void gos_rest_shutdown(emscripten_fetch_t* fetch) {
  /* Free data associated with the fetch */
  emscripten_fetch_close(fetch);
}
