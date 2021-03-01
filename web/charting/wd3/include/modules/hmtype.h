#ifndef WD3_HM_TYPE_C_MD_HM_H_
#define WD3_HM_TYPE_C_MD_HM_H_

#include <stdbool.h>

#include <SDL.h>

#include <gos/types.h>

#define WD3_HM_DOMAIN_TYPE_Y      1
#define WD3_HM_DOMAIN_TYPE_Z      2
#define WD3_HM_DOMAIN_TYPE_MASK   (WD3_HM_DOMAIN_TYPE_Y | WD3_HM_DOMAIN_TYPE_Z)
#define WD3_HM_DOMAIN_FROM        4
#define WD3_HM_DOMAIN_TO          8
#define WD3_HM_DOMAIN_MASK        (WD3_HM_DOMAIN_FROM | WD3_HM_DOMAIN_TO)

typedef struct wd3hm {
  int sdlinit;

  gos_screen screen;
  gos_rgb_gradient gradient;

  SDL_Surface* surface;

  gos_scale yscale;
  gos_scale zscale;

  /* Colors */
  int stops;
  Uint32 undefined;
  Uint32* separation;
  gos_rgb* stop;
  int* size;

  /* Data */
  int count;
  gos_point_2d* data;
  gos_point_2d** buffer;

} wd3hm;

#endif
