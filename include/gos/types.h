#ifndef GOS_TYPES_H_
#define GOS_TYPES_H_

typedef struct gos_point_2d {
  double x;
  double y;
} gos_point_2d;

typedef struct gos_range_1d {
  double from;
  double to;
} gos_range_1d;

typedef struct gos_range_2d {
  gos_range_1d first;
  gos_range_1d second;
} gos_range_2d;

typedef struct gos_generation_1d {
  gos_range_1d range;
  double noise;
} gos_generation_1d;

#endif
