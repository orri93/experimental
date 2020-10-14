#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <gos/data.h>
#include <gos/color.h>

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
