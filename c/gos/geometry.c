#include <assert.h>

#include <gos/types.h>
#include <gos/geometry.h>

double gos_geometry_distance_1d(gos_range_1d* r) {
  assert(r);
  return r->to - r->from;
}
