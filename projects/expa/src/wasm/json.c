#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <wasm/json.h>
#include <wasm/vector.h>

#define GOS_JSON_WS_TYPE_UPDATE_TEXT "update"
#define GOS_JSON_WS_TYPE_START_TEXT "start"
#define GOS_JSON_WS_TYPE_STOP_TEXT "stop"

bool gos_json_get_unsigned_integer(unsigned int* i, cJSON* j, const char* n) {
  bool result;
  double v;
  if ((result = gos_json_get_number(&v, j, n))) {
    *i = (int)v;
  }
  return result;
}

bool gos_json_get_number(double* v, cJSON* j, const char* n) {
  cJSON* c = NULL;
  if (j != NULL) {
    c = cJSON_GetObjectItemCaseSensitive(j, n);
    if (c != NULL) {
      if (cJSON_IsNumber(c)) {
        *v = cJSON_GetNumberValue(c);
        return true;
      }
    }
  }
  return false;
}

bool gos_json_points_to_vector(gos_point_2d_vector* v, cJSON* points) {
  int i;
  cJSON* p;
  bool result = true;
  if (cJSON_IsArray(points)) {
    v->count = cJSON_GetArraySize(points);
    v->points = calloc(v->count, sizeof(gos_point_2d));
    if(v->points != NULL) {
      for (i = 0; i < v->count; i++) {
        p = cJSON_GetArrayItem(points, i);
        if (cJSON_IsObject(p)) {
          if (!gos_json_get_number(&((v->points[i]).x), p, "x")) {
            result = false;
          }
          if (!gos_json_get_number(&((v->points[i]).y), p, "y")) {
            result = false;
          }
        }
      }
      return result;
    }
  }
  return false;
}

bool gos_json_to_vector(gos_point_2d_vector* v, cJSON* json) {
  cJSON* points;
  points = cJSON_GetObjectItemCaseSensitive(json, "p");
  if (points != NULL) {
    return gos_json_points_to_vector(v, points);
  } else {
    fprintf(stderr, "Failed to get p object from JSON\n");
  }
  return false;
}

bool gos_json_get_range(gos_range_1d* r, cJSON* rj) {
  bool result = true;
  if (!gos_json_get_number(&(r->from), rj, "f")) {
    result = false;
  }
  if (!gos_json_get_number(&(r->to), rj, "t")) {
    result = false;
  }
  return result;
}

bool gos_json_get_range_by_name(gos_range_1d* r, cJSON* rj, const char* n) {
  cJSON* range = cJSON_GetObjectItemCaseSensitive(rj, n);
  if (range != NULL) {
    if (cJSON_IsObject(range)) {
      return gos_json_get_range(r, range);
    }
  }
  return false;
}

bool gos_json_get_ranges(gos_expa_ranges* ranges, cJSON* m) {
  bool result = true;
  cJSON* rangesj = cJSON_GetObjectItemCaseSensitive(m, "r");
  if (rangesj != NULL) {
    if (cJSON_IsObject(rangesj)) {
      if (!gos_json_get_range_by_name(&(ranges->x), rangesj, "x")) {
        result = false;
      }
      if (!gos_json_get_range_by_name(&(ranges->y), rangesj, "y")) {
        result = false;
      }
      return result;
    }
  }
  return false;
}

bool gos_json_get_update(gos_json_ws_update* update, cJSON* message) {
  cJSON* t;
  cJSON* updatej;
  cJSON* vector;
  if (message != NULL) {
    updatej = cJSON_GetObjectItemCaseSensitive(message, "u");
    if (updatej != NULL) {
      if (cJSON_IsObject(updatej)) {
        gos_json_get_unsigned_integer(&(update->at), updatej, "t");
        vector = cJSON_GetObjectItemCaseSensitive(updatej, "v");
        if (vector != NULL && cJSON_IsObject(vector)) {
          return gos_json_to_vector(&(update->v), vector);
        }
      }
    }
  }
  return false;
}

bool gos_json_create_start(cJSON** start, int from) {
  cJSON* f;
  *start = cJSON_CreateObject();
  if (*start != NULL) {
    f = cJSON_CreateNumber((double)from);
    if (f != NULL) {
      return cJSON_AddItemToObject(*start, "f", f);
    }
  }
  return false;
}

bool gos_json_create_message(cJSON** message, GosWsMessageType type) {
  cJSON* t = NULL;
  *message = cJSON_CreateObject();
  if (*message != NULL) {
    switch (type) {
    case GosWsMsgUndefined:
      return false;
    case GosWsMsgStart:
      t = cJSON_CreateString("start");
      break;
    case GosWsMsgStop:
      t = cJSON_CreateString("stop");
      break;
    case GosWsMsgUpdate:
      t = cJSON_CreateString("update");
      break;
    case GosWsMsgUnknown:
      return false;
    default:
      return false;
    }
    if (t != NULL) {
      return cJSON_AddItemToObject(*message, "t", t);
    }
  }
  return false;
}

bool gos_json_create_start_message(cJSON** message, int from) {
  cJSON* start;
  if (gos_json_create_message(message, GosWsMsgStart)) {
    if (gos_json_create_start(&start, from)) {
      return cJSON_AddItemToObject(*message, "s", start);
    }
  }
  return false;
}

GosWsMessageType gos_json_get_message_type(cJSON* message) {
  char* s;
  cJSON* t;
  GosWsMessageType type = GosWsMsgUndefined;
  if (message != NULL) {
    t = cJSON_GetObjectItemCaseSensitive(message, "t");
    if (t != NULL) {
      if (cJSON_IsString(t) && (t->valuestring != NULL)) {
        if (strncmp(
          t->valuestring,
          GOS_JSON_WS_TYPE_UPDATE_TEXT,
          sizeof(GOS_JSON_WS_TYPE_UPDATE_TEXT)) == 0) {
          type = GosWsMsgUndefined;
        } else if (strncmp(
          t->valuestring,
          GOS_JSON_WS_TYPE_START_TEXT,
          sizeof(GOS_JSON_WS_TYPE_START_TEXT)) == 0) {
          type = GosWsMsgStart;
        } else if (strncmp(
          t->valuestring,
          GOS_JSON_WS_TYPE_STOP_TEXT,
          sizeof(GOS_JSON_WS_TYPE_STOP_TEXT)) == 0) {
          type = GosWsMsgStop;
        } else {
          type = GosWsMsgUnknown;
        }
      }
    }
  }
  return type;
}
