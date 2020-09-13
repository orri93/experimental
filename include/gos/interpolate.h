#ifndef GOS_INTERPOLATE_H_
#define GOS_INTERPOLATE_H_

/*
 *
 * Interpolation methods
 *
 * Written by Paul Bourke December 1999
 *
 * Estimated curve passes through all the given points. 
 *
 * See http://paulbourke.net/miscellaneous/interpolation/
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Linear interpolation is the simplest method of getting values at positions
 * in between the data points. The points are simply joined by straight line
 * segments.
 * For interpolated values between the two points mu ranges between 0 and 1.
 */
double gos_interpolate_linear(double y1, double y2, double mu);

/* A suitable orientated piece of a cosine function serves to provide a
 * smooth transition between adjacent segments.
 * For interpolated values between the two points mu ranges between 0 and 1.
 */
double gos_interpolate_cosine(double y1, double y2, double mu);

/* Cubic interpolation is the simplest method that offers true continuity
 * between the segments. As such it requires more than just the two endpoints of
 * the segment but also the two points on either side of them. So the function
 * requires 4 points in all labelled y0, y1, y2, and y3, in the code below.
 * mu still behaves the same way for interpolating between the segment y1 to y2
 */
double gos_interpolate_cubic(
  double y0,
  double y1,
  double y2,
  double y3,
  double mu);

/* Paul Breeuwsma proposes a different coefficients for a smoother interpolated
 * curve, which uses the slope between the previous point and the next as
 * the derivative at the current point. This results in what are generally
 * referred to as Catmull-Rom splines. */
double gos_interpolate_cubic_catmull_rom(
  double y0,
  double y1,
  double y2,
  double y3,
  double mu);

/*
 * Hermite interpolation like cubic requires 4 points so that it can achieve
 * a higher degree of continuity. In addition it has nice tension and
 * biasing controls. Tension can be used to tighten up the curvature at
 * the known points. The bias is used to twist the curve about the known points.
 * The examples shown here have the default tension and bias values of 0,
 * it will be left as an exercise for the reader to explore different tension
 * and bias values. */
double gos_interpolate_hermite(
  double y0,
  double y1,
  double y2,
  double y3,
  double mu,
  double tension,
  double bias);

#ifdef __cplusplus
}
#endif

#endif
