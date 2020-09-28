/*
 * Compilation
 * emcc -Os src/app/sdl-cube/sdl-chart.c -o src/assets/wasm/sdl-chart.js -s LEGACY_GL_EMULATION=1 -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']" -s MODULARIZE=1 -s EXPORT_NAME="ChartSdlModule"
 * 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <emscripten.h>

#include <SDL/SDL.h>

#include <gos/color.h>
#include <gos/noise.h>

#define GOS_COLOR_GRADIENT_STOPS 6
#define GOS_COLOR_GRADIENT_COLOR_COUNT 50
#define GOS_COLOR_GRADIENT_COUNT (GOS_COLOR_GRADIENT_STOPS - 1) * GOS_COLOR_GRADIENT_COLOR_COUNT

static gos_color_rgb _cstops[GOS_COLOR_GRADIENT_STOPS];
static gos_color_rgb** _cgradient = NULL;

static bool initialize();
static void shutdown();

static Uint32 to_sdl_color(SDL_PixelFormat* format, gos_color_rgb* rgb);

static void setpixel(SDL_Surface* surface, int x, int y, int width, Uint32 pixel);
static int sdlex(int width, int height);

int main(int argc, char** argv) {

  int i, mainresult = EXIT_SUCCESS;

  int width = 600;
  int height = 400;

  if(argc > 1) {
    width = atoi(argv[1]);
  }
  if(argc > 2) {
    height = atoi(argv[2]);
  }

  if(initialize()) {
    mainresult = sdlex(width, height);
  } else {
    mainresult = EXIT_FAILURE;
  }

  shutdown();

  return mainresult;
}

bool initialize(int width, int height) {
  gos_color_rgb** gradat;
  gos_color_rgb* rgbat;
  int i;

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
    return true;
  } else {
    return false;
  }
}

void shutdown() {
  int i;
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

int sdlex(int width, int height) {
  int ci;
  double noise;
  Uint32 pixel;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

  if (SDL_MUSTLOCK(screen)) {
    SDL_LockSurface(screen);
  }

  /* i is height and j is width */
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      noise = gos_noise_white(GOS_NOISE_DEFAULT_SEED, i, j);
      ci = GOS_COLOR_GRADIENT_COUNT * j / width;

      pixel = to_sdl_color(screen->format, _cgradient[ci]);
      setpixel(screen, j, i, width, pixel);
    }
  }

  if (SDL_MUSTLOCK(screen)) {
    SDL_UnlockSurface(screen);
  }
  SDL_Flip(screen);

  SDL_Quit();

  return 0;
}
