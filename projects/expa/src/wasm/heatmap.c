#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <emscripten/websocket.h>

#include <cJSON.h>
#endif

#include <gos/data.h>
#include <gos/color.h>
#include <gos/geometry.h>
#include <gos/interpolate.h>

#include <wasm/draw.h>
#include <wasm/chart.h>

#define GOS_HEATMAP_MESSAGE_SIZE 1024

#define GOS_HEATMAP_DEMO_WIDTH 600
#define GOS_HEATMAP_DEMO_HEIGHT 400

static SDL_Surface* _surface = NULL;
static gos_rgb_gradient _gradient = { NULL, 0 };
static gos_matrix _matrix = { NULL, 0, 0 };
static gos_vector _vd1 = { NULL, 0 };
static gos_vector _vd2 = { NULL, 0 };
static int _width = 0;
static int _height = 0;
static int _atstep = 0;

static char _gos_chart_message[GOS_HEATMAP_MESSAGE_SIZE];

static void gos_heatmap_create_random_vector(gos_vector* vector, double f);
static bool gos_heatmap_demo(int width, int height);
static bool gos_heatmap_initialize(int width, int height);
static void gos_heatmap_draw();
static void gos_heatmap_step();
static void gos_heatmap_line();
static void gos_heatmap_shutdown();

#ifdef GOS_CHART_SDL_MAIN
int SDL_main(int argc, char** argv) {
  return gos_heatmap_demo(GOS_HEATMAP_DEMO_WIDTH, GOS_HEATMAP_DEMO_HEIGHT) ?
    EXIT_SUCCESS : EXIT_FAILURE;
}
#else
int main(int argc, char** argv) {
  printf("Initialize the SDL Chart!\n");
  if (gos_heatmap_initialize(GOS_HEATMAP_DEMO_WIDTH, GOS_HEATMAP_DEMO_HEIGHT)) {
    gos_heatmap_draw();
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
#endif

#ifdef __EMSCRIPTEN__

bool gos_api_get_number(double* v, cJSON* j, const char* n) {
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

bool gos_api_update_from_json_to_vector(gos_point_2d_vector* v, cJSON* j) {
  int i;
  cJSON* p;
  bool result = true;
  if (cJSON_IsArray(j)) {
    v->count = cJSON_GetArraySize(j);
    v->points = calloc(v->count, sizeof(gos_point_2d));
    for (i = 0; i < v->count; i++) {
      p = cJSON_GetArrayItem(j, i);
      if (cJSON_IsObject(p)) {
        if (!gos_api_get_number(&((v->points[i]).x), p, "x")) {
          result = false;
        }
        if (!gos_api_get_number(&((v->points[i]).y), p, "y")) {
          result = false;
        }
      }
    }
    return result;
  }
  return false;
}

bool gos_vector_get_points(
  gos_point_2d_vector* v,
  gos_point_2d* p1,
  gos_point_2d* p2,
  int i) {
  if (i < v->count - 1) {
    p1->x = (v->points[i]).x;
    p1->y = (v->points[i]).y;
    p2->x = (v->points[i + 1]).x;
    p2->y = (v->points[i + 1]).y;
    return true;
  }
  return false;
}

bool gos_api_update_column_from_vector(
  gos_range_1d* xr,
  gos_range_1d* yr,
  gos_point_2d_vector* v,
  int i) {
  int j, index;
  Uint32 color;
  gos_point_2d p1, p2;
  double mu, r, f, n;
  if (i < _width) {
    for (j = 0; j < _height; j++) {
      index = gos_chart_vector_index(v, j);
      if(index >= 0) {
        r = (double)j;
        gos_vector_get_points(v, &p1, &p2, index);
        mu = (r - p1.x) / (p2.x - p1.x);
        f = gos_interpolate_linear(p1.y, p2.y, mu);
        n = (f - yr->from) / gos_geometry_distance_1d(yr);
        gos_draw_gradient_setpixel(_surface, &_gradient, i, j, _width, n);
      } else {
        gos_draw_setpixel(_surface, i, j, _width, 0);
      }
    }
    return true;
  }
  return false;
}

bool gos_api_update_column_from_json(
  gos_range_1d* xr,
  gos_range_1d* yr,
  cJSON* e,
  int i) {
  cJSON* points;
  gos_point_2d_vector vector;
  points = cJSON_GetObjectItemCaseSensitive(e, "p");
  if (points != NULL) {
    if (gos_api_update_from_json_to_vector(&vector, points)) {
      return gos_api_update_column_from_vector(xr, yr, &vector, i);
    }
  }
  return false;
}

bool gos_api_update_from_json(gos_range_1d* xr, gos_range_1d* yr, cJSON* m) {
  bool result = true;
  cJSON* e;
  cJSON* vectors = cJSON_GetObjectItemCaseSensitive(m, "v");
  int i = 0;
  if (vectors != NULL) {
    if (cJSON_IsArray(vectors)) {
      cJSON_ArrayForEach(e, vectors) {
        if (!gos_api_update_column_from_json(xr, yr, e, i++)) {
          result = false;
        }
      }
      return result;
    }
  }
  return false;
}

bool gos_api_get_range(gos_range_1d* r, cJSON* rj) {
  bool result = true;
  if (!gos_api_get_number(&(r->from), rj, "f")) {
    result = false;
  }
  if (!gos_api_get_number(&(r->to), rj, "t")) {
    result = false;
  }
  return result;
}

bool gos_api_get_range_by_name(gos_range_1d* r, cJSON* rj, const char* n) {
  cJSON* range = cJSON_GetObjectItemCaseSensitive(rj, n);
  if (range != NULL) {
    if (cJSON_IsObject(range)) {
      return gos_api_get_range(r, range);
    }
  }
  return false;
}

bool gos_api_get_ranges(gos_range_1d* xr, gos_range_1d* yr, cJSON* m) {
  bool result = true;
  cJSON* ranges = cJSON_GetObjectItemCaseSensitive(m, "r");
  if (ranges != NULL) {
    if (cJSON_IsObject(ranges)) {
      if (!gos_api_get_range_by_name(xr, ranges, "x")) {
        result = false;
      }
      if (!gos_api_get_range_by_name(yr, ranges, "y")) {
        result = false;
      }
      return result;
    }
  }
  return false;
}

void gos_api_succeeded(emscripten_fetch_t* fetch) {
  cJSON* v;
  cJSON* json;
  gos_range_1d xr, yr;
  const char* error_ptr;
  printf("Finished downloading %llu bytes from URL %s.\n",
    fetch->numBytes,
    fetch->url);
  /* The data is now available at
   * fetch->data[0] through fetch->data[fetch->numBytes-1];
   */
  if (fetch->numBytes > 0) {
    assert(fetch->data[fetch->numBytes - 1] == '\0');
    json = cJSON_Parse(fetch->data);
    if (json != NULL) {
      if (cJSON_IsObject(json)) {
        if (gos_api_get_ranges(&xr, &yr, json)) {
          if (!gos_api_update_from_json(&xr, &yr, json)) {
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

void gos_api_failed(emscripten_fetch_t* fetch) {
  printf(
    "Downloading %s failed, HTTP failure status code: %d.\n",
    fetch->url,
    fetch->status);
  /* Also free data on failure */
  emscripten_fetch_close(fetch);
}

EMSCRIPTEN_KEEPALIVE void start(const char* resturl, const char* wsurl) {
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.onsuccess = gos_api_succeeded;
  attr.onerror = gos_api_failed;
  emscripten_fetch(&attr, resturl);
}

EMSCRIPTEN_KEEPALIVE void stop() {
}

EMSCRIPTEN_KEEPALIVE void shutdown() {
  printf("Shutting down SDL Chart!\n");
  gos_heatmap_shutdown();
}

EMSCRIPTEN_KEEPALIVE void step() {
  gos_heatmap_step();
}

EMSCRIPTEN_KEEPALIVE void line() {
  gos_heatmap_line();
}

EMSCRIPTEN_KEEPALIVE void test(int value) {
  printf("Test value %d\n", value);
}
#endif

void gos_heatmap_create_random_vector(gos_vector* vector, double f) {
  int i;
  for (i = 0; i < vector->count; i++) {
    vector->data[i] = f * (((double)rand()) / RAND_MAX);
  }
}

bool gos_heatmap_demo(int width, int height) {
  bool move, go;
  SDL_Event event;

  if (gos_heatmap_initialize(width, height)) {
    gos_heatmap_draw();

    move = false;
    go = true;
    while (go) {
      if (move) {
        gos_heatmap_line();
      }

      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
          case SDLK_s:
            move = move ? false : true;
            break;
          case SDLK_q:
            go = false;
            break;
          default:
            break;
          }
          break;
        case SDL_QUIT:
          go = false;
          break;
        default:
          break;
        }
      }
    }

    //gos_heatmap_step();
    //gos_heatmap_step();
    //gos_heatmap_step();
    //gos_heatmap_step();

    gos_heatmap_shutdown();

    return true;
  }

  return false;
}

bool gos_heatmap_initialize(int width, int height) {
  int i;
  if (gos_data_initialize_matrix(&_matrix, width, height)) {
    if (gos_data_initialize_vector(&_vd1, height)) {
      if (gos_data_initialize_vector(&_vd2, width)) {
        if (gos_chart_create_gradient(&_gradient)) {

          for (i = 0; i < width; i++) {
            gos_heatmap_create_random_vector(&_vd1, 1.0);
            gos_data_set_vector_d1(&_matrix, &_vd1, i);
          }

          if (gos_draw_initialize(&_surface, width, height)) {
            _width = width;
            _height = height;
            return true;
          }
        }
      }
    }
  }
  return false;
}

void gos_heatmap_draw() {
  gos_draw_lock(_surface);
  gos_draw_matrix(_surface, &_matrix, &_gradient);
  gos_draw_unlock(_surface);
  SDL_Flip(_surface);
}

void gos_heatmap_step() {
  int i;
  switch (_atstep) {
  case 0:
    for (i = 0; i < 128; i++) {
      gos_heatmap_create_random_vector(&_vd1, 0.5);
      gos_data_shift_matrix_d1d1(&_matrix);
      gos_data_set_vector_d1(&_matrix, &_vd1, _matrix.countd1 - 2);
      gos_heatmap_draw();
    }
    _atstep++;
    break;
  case 1:
    for (i = 0; i < 64; i++) {
      gos_heatmap_create_random_vector(&_vd1, 0.5);
      gos_data_shift_matrix_d2d1(&_matrix);
      gos_data_set_vector_d1(&_matrix, &_vd1, 0);
      gos_heatmap_draw();
    }
    _atstep++;
    break;
  case 2:
    for (i = 0; i < 128; i++) {
      gos_heatmap_create_random_vector(&_vd2, 0.5);
      gos_data_shift_matrix_d1d2(&_matrix);
      gos_data_set_vector_d2(&_matrix, &_vd2, 0);
      gos_heatmap_draw();
    }
    _atstep++;
    break;
  case 3:
    for (i = 0; i < 64; i++) {
      gos_heatmap_create_random_vector(&_vd2, 0.5);
      gos_data_shift_matrix_d2d2(&_matrix);
      gos_data_set_vector_d2(&_matrix, &_vd2, _height - 1);
      gos_heatmap_draw();
    }
  default:
    _atstep = 0;
  }
}

void gos_heatmap_line() {
  gos_draw_lock(_surface);
  gos_heatmap_create_random_vector(&_vd1, 0.5);
  gos_draw_shift_d1d1(_surface);
  gos_draw_vector_d1(_surface, &_vd1, &_gradient, _surface->w - 1);
  gos_draw_unlock(_surface);
  SDL_Flip(_surface);
}

void gos_heatmap_shutdown() {
  gos_draw_shutdown();
  if (_gradient.gradient != NULL) {
    gos_color_free_rgb_gradient(&_gradient);
  }
  if (_vd2.data != NULL) {
    gos_data_free_vector(&_vd2);
  }
  if (_vd1.data != NULL) {
    gos_data_free_vector(&_vd1);
  }
  if (_matrix.data != NULL) {
    gos_data_free_matrix(&_matrix);
  }
}
