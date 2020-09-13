#include <gos/perlin.h>

#ifndef GOS_PERLIN_LERP_FUNCTION
/* Macro to linearly interpolate between a0 and a1 
 * Weight w should be in the range [0.0, 1.0] */
#define GOS_PERLIN_LERP_MACRO(a0,a1,w) ( (a0) + (w) * ( (a1) - (a0) ) )
#endif

#ifdef GOS_PERLIN_FLOAT
#ifdef GOS_PERLIN_LERP_FUNCTION
/* Function for float to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0] */
static float gos_perlin_lerp_f(float a0, float a1, float w);
#endif
/* Computes the dot product of the distance and gradient vectors as float */
static float gos_perlin_dot_grid_gradient_f(int ix, int iy, float x, float y);

/* Compute Perlin noise at coordinates x, y float version */
float gos_perlin_f(float x, float y) {
  float n0, n1, ix0, ix1, value;

  /* Determine grid cell coordinates */
  int x0 = (int)x;
  int x1 = x0 + 1;
  int y0 = (int)y;
  int y1 = y0 + 1;

  /* Determine interpolation weights
   * Could also use higher order polynomial/s-curve here */
  float sx = x - (float)x0;
  float sy = y - (float)y0;

  /* Interpolate between grid point gradients */
  n0 = gos_perlin_dot_grid_gradient_f(x0, y0, x, y);
  n1 = gos_perlin_dot_grid_gradient_f(x1, y0, x, y);
#ifdef GOS_PERLIN_LERP_FUNCTION
  ix0 = gos_perlin_lerp_f(n0, n1, sx);
#else
  ix0 = GOS_PERLIN_LERP_MACRO(n0, n1, sx);
#endif

  n0 = gos_perlin_dot_grid_gradient_f(x0, y1, x, y);
  n1 = gos_perlin_dot_grid_gradient_f(x1, y1, x, y);
#ifdef GOS_PERLIN_LERP_FUNCTION
  ix1 = gos_perlin_lerp_f(n0, n1, sx);
#else
  ix1 = GOS_PERLIN_LERP_MACRO(n0, n1, sx);
#endif

#ifdef GOS_PERLIN_LERP_FUNCTION
  value = gos_perlin_lerp_f(ix0, ix1, sy);
#else
  value = GOS_PERLIN_LERP_MACRO(ix0, ix1, sy);
#endif

  return value;
}
#endif

#ifdef GOS_PERLIN_DOUBLE
#ifdef GOS_PERLIN_LERP_FUNCTION
/* Function for double to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0] */
static double gos_perlin_lerp(double a0, double a1, double w);
#endif
/* Computes the dot product of the distance and gradient vectors as double */
static double gos_perlin_dot_grid_gradient(int ix, int iy, double x, double y);

/* Compute Perlin noise at coordinates x, y double version */
double gos_perlin(double x, double y) {
  double n0, n1, ix0, ix1, value;

  /* Determine grid cell coordinates */
  int x0 = (int)x;
  int x1 = x0 + 1;
  int y0 = (int)y;
  int y1 = y0 + 1;

  /* Determine interpolation weights
   * Could also use higher order polynomial/s-curve here */
  double sx = x - (double)x0;
  double sy = y - (double)y0;

  /* Interpolate between grid point gradients */
  n0 = gos_perlin_dot_grid_gradient(x0, y0, x, y);
  n1 = gos_perlin_dot_grid_gradient(x1, y0, x, y);
#ifdef GOS_PERLIN_LERP_FUNCTION
  ix0 = gos_perlin_lerp(n0, n1, sx);
#else
  ix0 = GOS_PERLIN_LERP_MACRO(n0, n1, sx);
#endif

  n0 = gos_perlin_dot_grid_gradient(x0, y1, x, y);
  n1 = gos_perlin_dot_grid_gradient(x1, y1, x, y);
#ifdef GOS_PERLIN_LERP_FUNCTION
  ix1 = gos_perlin_lerp(n0, n1, sx);
#else
  ix1 = GOS_PERLIN_LERP_MACRO(n0, n1, sx);
#endif

#ifdef GOS_PERLIN_LERP_FUNCTION
  value = gos_perlin_lerp(ix0, ix1, sy);
#else
  value = GOS_PERLIN_LERP_MACRO(ix0, ix1, sy);
#endif

  return value;
}
#endif


#ifdef GOS_PERLIN_FLOAT
#ifdef GOS_PERLIN_LERP_FUNCTION
/* Function for float to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0] */
float gos_perlin_lerp_f(float a0, float a1, float w) {
  return (1.0f - w) * a0 + w * a1;
}
#endif

/* Computes the dot product of the distance and gradient vectors as float */
float gos_perlin_dot_grid_gradient_f(int ix, int iy, float x, float y) {
  /* Precomputed (or otherwise) gradient vectors at each grid node */
}
#endif

#ifdef GOS_PERLIN_DOUBLE
#ifdef GOS_PERLIN_LERP_FUNCTION
/* Function for double to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0] */
double gos_perlin_lerp(double a0, double a1, double w) {
  return (1.0 - w) * a0 + w * a1;
}
#endif

/* Computes the dot product of the distance and gradient vectors as double */
double gos_perlin_dot_grid_gradient(int ix, int iy, double x, double y) {
  /* Precomputed (or otherwise) gradient vectors at each grid node */
}
#endif
