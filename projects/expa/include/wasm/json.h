#ifndef GOS_EXPA_WASM_JSON_H_
#define GOS_EXPA_WASM_JSON_H_

#include <stdbool.h>

/* https://github.com/DaveGamble/cJSON */
#include <cJSON.h>

#include <gos/types.h>

typedef enum GosWsMessageType {
  GosWsMsgUndefined,
  GosWsMsgStart,
  GosWsMsgStop,
  GosWsMsgUpdate,
  GosWsMsgUnknown
} GosWsMessageType;

#ifdef __cplusplus
extern "C" {
#endif

bool gos_json_get_number(double* v, cJSON* j, const char* n);
bool gos_json_to_vector(gos_point_2d_vector* v, cJSON* j);
bool gos_json_get_range(gos_range_1d* r, cJSON* rj);
bool gos_json_get_range_by_name(gos_range_1d* r, cJSON* rj, const char* n);
bool gos_json_get_ranges(gos_range_1d* xr, gos_range_1d* yr, cJSON* m);
bool gos_json_create_start(cJSON** start, int from);
bool gos_json_create_message(cJSON** message, GosWsMessageType type);
bool gos_json_create_start_message(cJSON** message, int from);

#ifdef __cplusplus
}
#endif

#endif
