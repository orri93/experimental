#include <modules/mset/map.h>

gosmf gmet_map_ratio(gosmrange* r, gosmf f) {
  return (f - r->f) / (r->t - r->f);
}

int gmset_map_to_screen(gosmrange* r, gosmf f, int m) {
  return (int)(gmet_map_ratio(r, f) * (gosmf)m);
}

void gmset_map_point_to_screen(
  gosmrect* r,
  gosmpoint* p,
  int* x,
  int* y,
  int w,
  int h) {
  *x = gmset_map_to_screen(&(r->a), p->x, w);
  *y = gmset_map_to_screen(&(r->b), p->y, h);
}
