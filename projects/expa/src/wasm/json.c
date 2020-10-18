#include <stdlib.h>

#include <wasm/json.h>
#include <wasm/vector.h>

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

bool gos_json_to_vector(gos_point_2d_vector* v, cJSON* j) {
  int i;
  cJSON* p;
  bool result = true;
  if (cJSON_IsArray(j)) {
    v->count = cJSON_GetArraySize(j);
    v->points = calloc(v->count, sizeof(gos_point_2d));
    if(v->points != NULL) {
      for (i = 0; i < v->count; i++) {
        p = cJSON_GetArrayItem(j, i);
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

bool gos_json_get_ranges(gos_range_1d* xr, gos_range_1d* yr, cJSON* m) {
  bool result = true;
  cJSON* ranges = cJSON_GetObjectItemCaseSensitive(m, "r");
  if (ranges != NULL) {
    if (cJSON_IsObject(ranges)) {
      if (!gos_json_get_range_by_name(xr, ranges, "x")) {
        result = false;
      }
      if (!gos_json_get_range_by_name(yr, ranges, "y")) {
        result = false;
      }
      return result;
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
