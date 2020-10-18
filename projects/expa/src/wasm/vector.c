#include <wasm/vector.h>

int gos_vector_index(gos_point_2d_vector* vector, int row) {
  int i;
  double r = row;
  for (i = 0; i < vector->count - 1; i++) {
    if (r >= (vector->points[i]).x && r <= (vector->points[i + 1]).x) {
      return i;
    }
  }
  return -1;
}

bool gos_vector_get_points(
  gos_point_2d_vector* v,
  gos_point_2d* p1,
  gos_point_2d* p2,
  int i) {
  if (i < v->count - 1) {
    p1->x = (v->points[i]).x;
    p1->y = (v->points[i]).y;
    p2->x = (v->points[i + 1]).x;
    p2->y = (v->points[i + 1]).y;
    return true;
  }
  return false;
}
