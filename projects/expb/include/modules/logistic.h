#ifndef GOS_EXPB_MODULES_LOGISTIC_MAP_H_
#define GOS_EXPB_MODULES_LOGISTIC_MAP_H_

#include <SDL2/SDL.h>

typedef struct gos_logistic_context {
  int sdlinit;
  int ttfinit;

  int w;
  int h;

  bool isgo;
  bool isdraw;

  SDL_Window* window;
  SDL_Renderer* renderer;

  SDL_Texture* graph;

} gos_logistic_context;

#endif
