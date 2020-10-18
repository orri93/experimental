#include <gos/geometry.h>
#include <gos/interpolate.h>

#include <wasm/update.h>
#include <wasm/vector.h>
#include <wasm/draw.h>
#include <wasm/json.h>

bool gos_update_column_from_vector(
  gos_expa_data* expad,
  gos_range_1d* xr,
  gos_range_1d* yr,
  gos_point_2d_vector* v,
  int i) {
  SDL_Surface* surface;
  int j, index;
  gos_point_2d p1, p2;
  double mu, r, f, n;
  surface = expad->surface;
  if (i < surface->w) {
    gos_draw_lock(surface);
    for (j = 0; j < surface->h; j++) {
      index = gos_vector_index(v, j);
      if (index >= 0) {
        r = (double)j;
        gos_vector_get_points(v, &p1, &p2, index);
        mu = (r - p1.x) / (p2.x - p1.x);
        f = gos_interpolate_linear(p1.y, p2.y, mu);
        n = (f - yr->from) / gos_geometry_distance_1d(yr);
        gos_draw_gradient_setpixel(expad, i, j, surface->w, n);
      } else {
        gos_draw_setpixel(surface, i, j, surface->w, 0);
      }
    }
    gos_draw_unlock(surface);
    SDL_Flip(surface);
    return true;
  } else {
    return false;
  }
}

bool gos_update_column_from_json(
  gos_expa_data* expad,
  gos_range_1d* xr,
  gos_range_1d* yr,
  cJSON* e,
  int i) {
  cJSON* points;
  gos_point_2d_vector vector;
  points = cJSON_GetObjectItemCaseSensitive(e, "p");
  if (points != NULL) {
    if (gos_json_to_vector(&vector, points)) {
      return gos_update_column_from_vector(expad, xr, yr, &vector, i);
    }
  }
  return false;
}

bool gos_update_from_json(
  gos_expa_data* expad,
  gos_range_1d* xr,
  gos_range_1d* yr,
  cJSON* m) {
  bool result = true;
  cJSON* e;
  cJSON* vectors = cJSON_GetObjectItemCaseSensitive(m, "v");
  int i = 0;
  if (vectors != NULL) {
    if (cJSON_IsArray(vectors)) {
      cJSON_ArrayForEach(e, vectors) {
        if (!gos_update_column_from_json(expad, xr, yr, e, i++)) {
          result = false;
        }
      }
      return result;
    }
  }
  return false;
}
