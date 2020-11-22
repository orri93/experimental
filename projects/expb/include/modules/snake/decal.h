#ifndef DECAL_H
#define DECAL_H

#include <modules/snake/types.h>

decal *decal_init(
  decal *d,
  SDL_Texture *texture,
  double x,
  double y,
  double w,
  double h);

void decal_set_xywh(
  decal *d,
  double x,
  double y,
  double w,
  double h);

#endif
