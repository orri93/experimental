#ifndef GOS_EXP_SDL_RENDER_H_
#define GOS_EXP_SDL_RENDER_H_

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct GosSdlExpChartVector {
  int width, height;
};

typedef struct GosSdlExpChart {

  SDL_Surface* surface;

  bool issurfacelocked;

  int width, height;

  double minx, maxx, miny, maxy, minz, maxz;

} GosSdlExpChart;



bool gos_sdl_exp_render(GosSdlExpChart* chart);

#endif
