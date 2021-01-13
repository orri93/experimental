#ifndef GOS_D3NG_MD_HM_H_
#define GOS_D3NG_MD_HM_H_

#include <stdbool.h>

#include <SDL.h>

#include <gos/color.h>

typedef struct goshm {
  int sdlinit;

  bool isgo;  /* Only used by the desktop demo */

  int width;
  int height;

  gos_rgb_gradient gradient;

  SDL_Window* window;
  SDL_Renderer* renderer;

  SDL_Texture* texture;
} goshm;

void ghm_initialize(goshm* context, int width, int height);
void ghm_parse_argument(goshm* context, int argc, char** argv);
bool ghm_create(goshm* context, int x, int y, const char* title);
bool ghm_create_texture(goshm* context);
bool ghm_draw(goshm* context);
bool ghm_loop(goshm* context);
bool ghm_shift(goshm* context);
void ghm_shutdown(goshm* context);

#endif
