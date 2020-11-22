#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

bool engine_init(
  const char* imgdir,
  const char* fontdir,
  unsigned int w,
  unsigned int h);

void engine_destroy();

bool engine_start();

void engine_reset();

void engine_get_grid_coord(const int *pixel_coord, int *grid_coord);
void engine_get_pixel_coord(const int *grid_coord, int *pixel_coord);
void engine_apply_boundary_conditions(int *grid_coords);

#endif
