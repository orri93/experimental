#include <modules/snake/decal.h>

decal *decal_init(
  decal *d,
  SDL_Texture *texture,
  double x,
  double y,
  double w,
  double h) {
  d->texture = texture;
  decal_set_xywh(d, x, y, w, h);
  return d;
}

void decal_set_xywh(
  decal *d,
  double x,
  double y,
  double w,
  double h) {
  d->rect.x = (int)x;
  d->rect.y = (int)y;
  d->rect.w = (int)w;
  d->rect.h = (int)h;
}
