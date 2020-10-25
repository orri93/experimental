#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <wasm/rest.h>
#include <wasm/ws.h>
#endif

#include <gos/data.h>
#include <gos/color.h>

#include <wasm/ws.h>
#include <wasm/draw.h>
#include <wasm/chart.h>
#include <wasm/types.h>
#include <wasm/heatmap.h>
#include <wasm/performance.h>

#define GOS_HEATMAP_MESSAGE_SIZE 1024

#define GOS_HEATMAP_DEMO_WIDTH 600
#define GOS_HEATMAP_DEMO_HEIGHT 400

#define GOS_HEATMAP_PERFORMANCE_SIZE 1024


gos_expa_data _expa_data = {
  NULL,                           /* surface  */
  { { 0.0, 0.0 }, { 0.0, 0.0 } }, /* ranges   */
  { NULL, 0 },                    /* gradient */ 
  { 0, 0 }                        /* ws       */
};

gos_performance _performance;
gos_performance _interval_performance;

static gos_matrix _matrix = { NULL, 0, 0 };
static gos_vector _vd1 = { NULL, 0 };
static gos_vector _vd2 = { NULL, 0 };

static int _atstep = 0;

static char _gos_chart_message[GOS_HEATMAP_MESSAGE_SIZE];
static int _gos_chart_message_len = 0;

#ifdef GOS_CHART_SDL_MAIN
int SDL_main(int argc, char** argv) {
  return gos_heatmap_demo(GOS_HEATMAP_DEMO_WIDTH, GOS_HEATMAP_DEMO_HEIGHT) ?
    EXIT_SUCCESS : EXIT_FAILURE;
}
#else
int main(int argc, char** argv) {
  printf("Initialize the GOS Heatmap!\n");
  if (gos_heatmap_initialize(GOS_HEATMAP_DEMO_WIDTH, GOS_HEATMAP_DEMO_HEIGHT)) {
    gos_heatmap_draw();
    if (gos_performance_initialize_sd(
      &_performance,
      GOS_HEATMAP_PERFORMANCE_SIZE)) {
      if (gos_performance_initialize_sd(
        &_interval_performance,
        GOS_HEATMAP_PERFORMANCE_SIZE)) {
        printf("The GOS Heatmap has been initialized\n");
        return EXIT_SUCCESS;
      } else {
        fprintf(stderr, "Failed to initialized the interval performance\n");
      }
    } else {
      fprintf(stderr, "Failed to initialized the performance\n");
    }
  }
  return EXIT_FAILURE;
}
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE void fetchrest(const char* url) {
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.userData = &_expa_data;
  attr.onsuccess = gos_rest_succeeded;
  attr.onerror = gos_rest_failed;
  emscripten_fetch(&attr, url);
}

EMSCRIPTEN_KEEPALIVE void shutdown() {
  printf("Shutting down SDL Chart!\n");
  gos_heatmap_shutdown();
  if (_expa_data.ws.socket > 0) {
    printf("Shutting down the Web Socket!\n");
    gos_ws_delete(&(_expa_data.ws));
  }
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

EMSCRIPTEN_KEEPALIVE void lock() {
  gos_draw_lock(_expa_data.surface);
}

EMSCRIPTEN_KEEPALIVE void unlock_and_flip() {
  gos_draw_unlock(_expa_data.surface);
  SDL_Flip(_expa_data.surface);
}

EMSCRIPTEN_KEEPALIVE void shift() {
  gos_draw_shift_d1d1(_expa_data.surface);
}

EMSCRIPTEN_KEEPALIVE void unset_pixel_last_column(int y) {
  gos_draw_setpixel(_expa_data.surface, _expa_data.surface->w - 1, y, 0);
}

EMSCRIPTEN_KEEPALIVE void set_pixel_last_column(int y, double v) {
  gos_draw_gradient_setpixel(&_expa_data, _expa_data.surface->w - 1, y, v);
} 

EMSCRIPTEN_KEEPALIVE void set_pixel(int x, int y, double v) {
  gos_draw_gradient_setpixel(&_expa_data, x, y, v);
}

EMSCRIPTEN_KEEPALIVE void unset_pixel(int x, int y) {
  gos_draw_setpixel(_expa_data.surface, x, y, 0);
}

EMSCRIPTEN_KEEPALIVE int set_data(int* values, int count) {
  int i, s = 0;
  for(i = 0; i < count; i++) {
    printf("Adding %d to the sum\n", values[i]);
    s += values[i];
  }
  _gos_chart_message_len = snprintf(
    _gos_chart_message,
    GOS_HEATMAP_MESSAGE_SIZE,
    "Set data value sum: %d",
    s);
  return s;
}

EMSCRIPTEN_KEEPALIVE void advanced_start(const char* url, const char* ws, int at) {
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  _expa_data.ws.start_from = at;
  strncpy(_expa_data.ws.url, ws, GOS_EXPA_WASM_WS_URL);
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.userData = &_expa_data;
  attr.onsuccess = gos_rest_advanced_succeeded;
  attr.onerror = gos_rest_failed;
  printf(
    "Advanced start from %d with Rest API URL as %s and Web Socket URL as %s\n",
    at, url, _expa_data.ws.url);
  emscripten_fetch(&attr, url);
}

EMSCRIPTEN_KEEPALIVE void advanced_stop() {
  if (_expa_data.ws.socket > 0) {
    gos_ws_send_stop(&(_expa_data.ws));
  }
}

EMSCRIPTEN_KEEPALIVE void advanced_close() {
  if (_expa_data.ws.socket > 0) {
    gos_ws_close(&(_expa_data.ws), 1000);
  }
}

#endif

void gos_heatmap_create_random_vector(gos_vector* vector, double f) {
  int i;
  for (i = 0; i < vector->count; i++) {
    vector->data[i] = f * (((double)rand()) / RAND_MAX);
  }
}

bool gos_heatmap_initialize(int width, int height) {
  int i;
  if (gos_data_initialize_matrix(&_matrix, width, height)) {
    if (gos_data_initialize_vector(&_vd1, height)) {
      if (gos_data_initialize_vector(&_vd2, width)) {
        if (gos_chart_create_gradient(&(_expa_data.gradient))) {

          for (i = 0; i < width; i++) {
            gos_heatmap_create_random_vector(&_vd1, 1.0);
            gos_data_set_vector_d1(&_matrix, &_vd1, i);
          }

          if (gos_draw_initialize(&_expa_data.surface, width, height)) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

void gos_heatmap_draw() {
  gos_draw_lock(_expa_data.surface);
  gos_draw_matrix(&_expa_data, &_matrix);
  gos_draw_unlock(_expa_data.surface);
  SDL_Flip(_expa_data.surface);
}

void gos_heatmap_step() {
  int i;
  int height = _expa_data.surface->h;
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
      gos_data_set_vector_d2(&_matrix, &_vd2, height - 1);
      gos_heatmap_draw();
    }
  default:
    _atstep = 0;
  }
}

void gos_heatmap_line() {
  gos_draw_lock(_expa_data.surface);
  gos_heatmap_create_random_vector(&_vd1, 0.5);
  gos_draw_shift_d1d1(_expa_data.surface);
  gos_draw_vector_d1(&_expa_data, &_vd1, _expa_data.surface->w - 1);
  gos_draw_unlock(_expa_data.surface);
  SDL_Flip(_expa_data.surface);
}

void gos_heatmap_shutdown() {
  gos_performance_shutdown(&_performance);
  gos_performance_shutdown(&_interval_performance);

  gos_draw_shutdown();
  if ((_expa_data.gradient).gradient != NULL) {
    gos_color_free_rgb_gradient(&(_expa_data.gradient));
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
