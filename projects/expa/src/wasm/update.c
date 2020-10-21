#include <stdio.h>

#include <SDL.h>

#include <gos/geometry.h>
#include <gos/interpolate.h>

#include <wasm/update.h>
#include <wasm/vector.h>
#include <wasm/draw.h>
#include <wasm/json.h>

bool gos_update_last_column_from_vector(
  gos_expa_data* ed,
  gos_point_2d_vector* v) {
  return gos_update_column_from_vector(ed, v, ed->surface->w - 1);
}

bool gos_update_column_from_vector(
  gos_expa_data* expad,
  gos_point_2d_vector* v,
  int i) {
  SDL_Surface* surface;
  int j, index;
  gos_point_2d p1, p2;
  gos_range_1d* yr;
  double mu, r, f, n;
  yr = &((expad->ranges).y);
  surface = expad->surface;
  if (gos_geometry_distance_1d(yr) != 0.0) {
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
          gos_draw_gradient_setpixel(expad, i, j, n);
        } else {
          gos_draw_setpixel(surface, i, j, 0);
        }
      }
      gos_draw_unlock(surface);
      SDL_Flip(surface);
      return true;
    } else {
      fprintf(stderr, "The index of %d is out of range of %d\n", i, surface->w);
    }
  } else {
    fprintf(stderr, "The distance for the Y range is zero\n");
  }
  return false;
}

bool gos_update_from_json(gos_expa_data* expad, cJSON* m) {
  int i, vc;
  bool result;
  gos_point_2d_vector vector;
  cJSON* e;
  cJSON* vectors = cJSON_GetObjectItemCaseSensitive(m, "v");
  if (vectors != NULL) {
    if (cJSON_IsArray(vectors)) {
      i = 0;
      result = true;
      vc = cJSON_GetArraySize(vectors);
      printf("Updating from JSON with %d vector count\n", vc);
      cJSON_ArrayForEach(e, vectors) {
        if (gos_json_to_vector(&vector, e)) {
          if (!gos_update_column_from_vector(expad, &vector, i)) {
            result = false;
          }
        } else {
          result = false;
        }
      }
      return result;
    } else {
      fprintf(stderr, "Failed to get v object from JSON\n");
    }
  } else {
    fprintf(stderr, "Failed to get v object from JSON\n");
  }
  return false;
}
