#ifndef GOS_EXPB_MODULES_MSET_H_
#define GOS_EXPB_MODULES_MSET_H_

#include <SDL2/SDL.h>

#include <gos/color.h>

#include <modules/mc/mandel.h>

#define GOS_MSET_HALF 0.5L
#define GOS_MSET_MAX_F 4.0L

#define GOS_MSET_STEP 0.01L
#define GOS_MSET_RESIZE_RATIO 0.9L

typedef struct gos_mset_context {
  int sdlinit;
  int ttfinit;

  int w;
  int h;

  gosmrect mrect;

  gosmpoint mcpoint;

  bool isgo;
  bool isdraw;
  bool isdrawcross;
  bool isresize;

  int gstepsize;
  bool colorminit;
  gos_rgb_gradient gradient;

  SDL_Window* window;
  SDL_Renderer* renderer;

  SDL_Texture* set;

} gos_mset_context;

#endif
