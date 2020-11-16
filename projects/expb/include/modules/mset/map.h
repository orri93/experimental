#ifndef GOS_EXPB_MODULES_MSET_MAP_H_
#define GOS_EXPB_MODULES_MSET_MAP_H_

#include <stdbool.h>

#include <modules/mset.h>

#ifdef __cplusplus
extern "C" {
#endif

gosmf gmet_map_ratio(gosmrange* r, gosmf f);
int gmset_map_to_screen(gosmrange* r, gosmf f, int m);
void gmset_map_point_to_screen(
  gosmrect* r,
  gosmpoint* p,
  int* x,
  int* y,
  int w,
  int h);

#ifdef __cplusplus
}
#endif

#endif
