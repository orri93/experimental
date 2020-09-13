#include <assert.h>

#include <gos/geometry.h>
#include <gos/signal.h>

double gos_signal_random_range(gos_range_1d* r) {
  assert(r);
  double distance;
  distance = gos_geometry_distance_1d(r);
  return r->from + GOS_SIGNAL_RANDOM_DOUBLE(distance);
}

void gos_signal_generate_range_random_y(
  double* values,
  int count,
  double from,
  double to) {
  int i;
  double yd;
  yd = to - from;
  assert(values != NULL);
  for (i = 0; i < count; i++) {
    *values = from + GOS_SIGNAL_RANDOM_DOUBLE(yd);
    values++;
  }
}

void gos_signal_generate_2d_range_random_y(
  gos_point_2d* v,
  gos_range_2d* r,
  int c) {
  int i;
  double x, y, xd, yd, xs;
  assert(v != NULL);
  assert(r != NULL);
  xd = gos_geometry_distance_1d(&(r->first));
  yd = gos_geometry_distance_1d(&(r->second));
  x = xs = xd / (1.0 + (double)c);
  for (i = 0; i < c; i++) {
    y = r->second.from + GOS_SIGNAL_RANDOM_DOUBLE(yd);
    v->x = x;
    v->y = y;
    v++;
    x += xs;
  }
}

void gos_signal_generate_2d_range_random_x_and_y(
  gos_point_2d* v,
  gos_range_2d* r,
  int c) {
  int i;
  double x, y, xa, xd, yd, xs;
  gos_range_1d xr;
  assert(v != NULL);
  assert(r != NULL);
  xd = gos_geometry_distance_1d(&(r->first));
  yd = gos_geometry_distance_1d(&(r->second));
  xa = xs = xd / (1.0 + (double)c);
  xr.to = xs / 2.0;
  xr.from = -xr.to;
  for (i = 0; i < c; i++) {
    x = xa + gos_signal_random_range(&xr);
    y = r->second.from + GOS_SIGNAL_RANDOM_DOUBLE(yd);
    v->x = x;
    v->y = y;
    v++;
    xa += xs;
  }
}
