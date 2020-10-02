/*
 * Compilation
 * emcc -Os src/app/sdl-cube/sdl-chart.c -o src/assets/wasm/sdl-chart.js -s LEGACY_GL_EMULATION=1 -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']" -s MODULARIZE=1 -s EXPORT_NAME="ChartSdlModule"
 * 
 */

#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <emscripten.h>

#include <SDL/SDL.h>

#include <gos/color.h>
#include <gos/noise.h>

#define GOS_SDL_CHART_MESSAGE_SIZE 1024

#define GOS_DATA_INIT_MAX 100
#define GOS_DATA_LINE_MAX 50

#define GOS_COLOR_GRADIENT_STOPS 6
#define GOS_COLOR_GRADIENT_COLOR_COUNT 50
#define GOS_COLOR_GRADIENT_COUNT (GOS_COLOR_GRADIENT_STOPS - 1) * GOS_COLOR_GRADIENT_COLOR_COUNT

static char _gos_sdl_chart_message[GOS_SDL_CHART_MESSAGE_SIZE];

static int _width = 0;
static int _height = 0;

static int _sdl_init_result = 1;

static gos_color_rgb _cstops[GOS_COLOR_GRADIENT_STOPS];
static gos_color_rgb** _cgradient = NULL;

static int** _data_matrix = NULL;

static SDL_Surface* _surface = NULL;

static bool initialize(int width, int height);

static Uint32 to_sdl_color(SDL_PixelFormat* format, gos_color_rgb* rgb);

static void setpixel(SDL_Surface* surface, int x, int y, int width, Uint32 pixel);

static bool draw();

int main(int argc, char** argv) {
  int mainresult = EXIT_SUCCESS;

  int width = 600;
  int height = 400;

  if(argc > 1) {
    width = atoi(argv[1]);
  }
  if(argc > 2) {
    height = atoi(argv[2]);
  }

  if(initialize(width, height)) {
    if(!draw()) {
      mainresult = EXIT_FAILURE;
    }
  } else {
    fprintf(stderr, "%s\n", _gos_sdl_chart_message);
    mainresult = EXIT_FAILURE;
  }

  return mainresult;
}

EMSCRIPTEN_KEEPALIVE const char* message() {
  return (const char*)_gos_sdl_chart_message;
}

EMSCRIPTEN_KEEPALIVE void shutdown() {
  int i;
  printf("Shutting down SDL Chart!\n");
  if(_cgradient != NULL) {
    for(i = 0; i < GOS_COLOR_GRADIENT_COUNT; i++) {
      if(_cgradient[i] != NULL) {
        free(_cgradient[i]);
        _cgradient[i] = NULL;
      }
    }
    free(_cgradient);
    _cgradient = NULL;
  }
  if(_data_matrix) {
    for(i = 0; i < _width; i++) {
      if(_cgradient[i] != NULL) {
        free(_cgradient[i]);
        _cgradient[i] = NULL;
      }
    }
  }

  _width = 0;
  _height = 0;

  if(_sdl_init_result == 0) {
    printf("Quitting SDL!\n");
    SDL_Quit();
    _sdl_init_result = 1;
  } else {
    printf("Never mind SDL!\n");
  }
}

EMSCRIPTEN_KEEPALIVE void addline() {
  draw();
}

bool initialize(int width, int height) {
  gos_color_rgb** gradat;
  gos_color_rgb* rgbat;
  int i, j;
  int *row;

  memset(_gos_sdl_chart_message, 0, GOS_SDL_CHART_MESSAGE_SIZE);

  _width = width;
  _height = height;

  rgbat = _cstops;
  gos_color_assign_rgb32(rgbat++, 0x07049b);  /* Dark blue */
  gos_color_assign_rgb32(rgbat++, 0x02f7f3);  /* Cyan */
  gos_color_assign_rgb32(rgbat++, 0x09f725);  /* Green */
  gos_color_assign_rgb32(rgbat++, 0xf4ec04);  /* Yellow */
  gos_color_assign_rgb32(rgbat++, 0xf79d01);  /* Orange */
  gos_color_assign_rgb32(rgbat++, 0x8c0101);  /* Dark red */
  
  _cgradient = (gos_color_rgb**)calloc(
    GOS_COLOR_GRADIENT_COUNT,
    sizeof(gos_color_rgb*));
  if(_cgradient != NULL) {
    for(i = 0; i < GOS_COLOR_GRADIENT_COUNT; i++) {
      _cgradient[i] = (gos_color_rgb*)calloc(1, sizeof(gos_color_rgb));
      if(_cgradient[i] == NULL) {
        snprintf(
          _gos_sdl_chart_message,
          GOS_SDL_CHART_MESSAGE_SIZE,
          "Out of memory at color gradient no. %d",
          i);
        return false;
      }
    }

    rgbat = _cstops;
    gradat = _cgradient;
    for(i = 0; i < GOS_COLOR_GRADIENT_STOPS - 1; i++) {
      gos_color_perceptual_steps(
        gradat,
        rgbat,
        rgbat + 1,
        GOS_COLOR_GAMMA,
        GOS_COLOR_GRADIENT_COLOR_COUNT);
      gradat += GOS_COLOR_GRADIENT_COLOR_COUNT;
      rgbat++;
    }

    _data_matrix = (int**)calloc(width, sizeof(int*));
    if(_data_matrix != NULL) {
      for(i = 0; i < width; i++) {
        _data_matrix[i] = (int*)calloc(1, sizeof(int));
        if(_data_matrix[i] == NULL) {
          snprintf(
            _gos_sdl_chart_message,
            GOS_SDL_CHART_MESSAGE_SIZE,
            "Out of memory at data matrix no. %d",
            i);
        }
        row = _data_matrix[i];
        for(j = 0; j < height; j++) {
          *(row++) = gos_noise_random_maximum_integer(GOS_DATA_INIT_MAX);
        }
      }

      if((_sdl_init_result = SDL_Init(SDL_INIT_VIDEO)) == 0) {
        _surface = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
        if(_surface != NULL) {
          return true;
        } else {
          snprintf(
            _gos_sdl_chart_message,
            GOS_SDL_CHART_MESSAGE_SIZE,
            "SDL Set Video Mode failed");
        }
      } else {
        snprintf(
          _gos_sdl_chart_message,
          GOS_SDL_CHART_MESSAGE_SIZE,
          "SDL initialization failed with code %d and reason: %s",
          _sdl_init_result,
          SDL_GetError());
      }

    } else {
      snprintf(
        _gos_sdl_chart_message,
        GOS_SDL_CHART_MESSAGE_SIZE,
        "Out of memory for data matrix");
    }
  } else {
    snprintf(
      _gos_sdl_chart_message,
      GOS_SDL_CHART_MESSAGE_SIZE,
      "Out of memory for color gradient");
  }
  return false;
}

Uint32 to_sdl_color(SDL_PixelFormat* format, gos_color_rgb* rgb) {
  return SDL_MapRGB(
    format,
    (Uint8)(rgb->r),
    (Uint8)(rgb->g),
    (Uint8)(rgb->b));
}

void setpixel(SDL_Surface* surface, int x, int y, int width, Uint32 pixel) {
  *(((Uint32*)(surface->pixels)) + x + y * width) = pixel;
}

bool draw() {
  int ci, i, j;
  Uint32 pixel;

  if (SDL_MUSTLOCK(_surface)) {
    SDL_LockSurface(_surface);
  }

  for(i = 0; i < _width; i++) {
    ci = GOS_COLOR_GRADIENT_COUNT * i / _width;
    pixel = to_sdl_color(_surface->format, _cgradient[ci]);
    for(j = 0; j < _height; j++) {
      setpixel(_surface, i, j, _width, pixel);
    }
  }
  
  if (SDL_MUSTLOCK(_surface)) {
    SDL_UnlockSurface(_surface);
  }
  SDL_Flip(_surface);

  return true;
}
