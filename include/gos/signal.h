#ifndef GOS_SIGNAL_H_
#define GOS_SIGNAL_H_

#include <stdlib.h>

#include <gos/types.h>

#define GOS_SIGNAL_RANDOM_DOUBLE(m) (m) * (double)rand() / (double)RAND_MAX;

#ifdef __cplusplus
extern "C" {
#endif

double gos_signal_random_range(gos_range_1d* r);

void gos_signal_generate_range_random_y(
  double* values,
  int count,
  double from,
  double to);

void gos_signal_generate_2d_range_random_y(
  gos_point_2d* v,
  gos_range_2d* r,
  int c);

void gos_signal_generate_2d_range_random_x_and_y(
  gos_point_2d* v,
  gos_range_2d* r,
  int c);

#ifdef __cplusplus
}
#endif

#endif
