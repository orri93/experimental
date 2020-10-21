#ifndef GOS_EXPA_WASM_JSON_H_
#define GOS_EXPA_WASM_JSON_H_

#include <stdbool.h>

/* https://github.com/DaveGamble/cJSON */
#include <cJSON.h>

#include <wasm/types.h>

typedef enum GosWsMessageType {
  GosWsMsgUndefined,
  GosWsMsgStart,
  GosWsMsgStop,
  GosWsMsgUpdate,
  GosWsMsgUnknown
} GosWsMessageType;

typedef struct gos_json_ws_update {
  gos_point_2d_vector v;
  unsigned int at;
} gos_json_ws_update;

#ifdef __cplusplus
extern "C" {
#endif

bool gos_json_get_unsigned_integer(unsigned int* i, cJSON* j, const char* n);
bool gos_json_get_number(double* v, cJSON* j, const char* n);
bool gos_json_points_to_vector(gos_point_2d_vector* v, cJSON* points);
bool gos_json_to_vector(gos_point_2d_vector* v, cJSON* json);
bool gos_json_get_range(gos_range_1d* r, cJSON* rj);
bool gos_json_get_range_by_name(gos_range_1d* r, cJSON* rj, const char* n);
bool gos_json_get_ranges(gos_expa_ranges* ranges, cJSON* m);
bool gos_json_get_update(gos_json_ws_update* update, cJSON* message);
bool gos_json_create_start(cJSON** start, int from);
bool gos_json_create_message(cJSON** message, GosWsMessageType type);
bool gos_json_create_start_message(cJSON** message, int from);

GosWsMessageType gos_json_get_message_type(cJSON* message);

#ifdef __cplusplus
}
#endif

#endif
