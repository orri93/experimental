#include <assert.h>
#include <stdio.h>

#include <wasm/rest.h>
#include <wasm/json.h>
#include <wasm/types.h>
#include <wasm/update.h>

void gos_rest_succeeded(emscripten_fetch_t* fetch) {
  gos_expa_data* expad;
  cJSON* v;
  cJSON* json;
  gos_range_1d xr, yr;
  const char* error_ptr;
  printf("Finished downloading %llu bytes from URL %s.\n",
    fetch->numBytes,
    fetch->url);

  expad = (gos_expa_data*)(fetch->userData);
  assert(expad != NULL);

  /* The data is now available at
   * fetch->data[0] through fetch->data[fetch->numBytes-1];
   */
  if (fetch->numBytes > 0) {
    json = cJSON_ParseWithLength(fetch->data, (size_t)(fetch->numBytes));
    if (json != NULL) {
      if (cJSON_IsObject(json)) {
        if (gos_json_get_ranges(&xr, &yr, json)) {
          if (!gos_update_from_json(expad, &xr, &yr, json)) {
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
      error_ptr = cJSON_GetErrorPtr();
      if (error_ptr != NULL) {
        fprintf(stderr, "Error before: %s\n", error_ptr);
      }
    }
  }

  /* Free data associated with the fetch */
  emscripten_fetch_close(fetch);
}

void gos_rest_failed(emscripten_fetch_t* fetch) {
  printf(
    "Downloading %s failed, HTTP failure status code: %d.\n",
    fetch->url,
    fetch->status);
  /* Also free data on failure */
  emscripten_fetch_close(fetch);
}
