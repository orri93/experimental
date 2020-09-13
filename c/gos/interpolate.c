#include <math.h>

#include <gos/interpolate.h>

double gos_interpolate_linear(double y1, double y2, double mu) {
  return y1 * (1.0 - mu) + y2 * mu;
}

double gos_interpolate_cosine(double y1, double y2, double mu) {
  double mu2;
  mu2 = (1.0 - cos(mu * M_PI)) / 2.0;
  return y1 * (1.0 - mu2) + y2 * mu2;
}

double gos_interpolate_cubic(
  double y0,
  double y1,
  double y2,
  double y3,
  double mu) {
  double a0, a1, a2, a3, mu2;
  mu2 = mu * mu;
  a0 = y3 - y2 - y0 + y1;
  a1 = y0 - y1 - a0;
  a2 = y2 - y0;
  a3 = y1;
  return a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3;
}

double gos_interpolate_cubic_catmull_rom(
  double y0,
  double y1,
  double y2,
  double y3,
  double mu) {
  double a0, a1, a2, a3, mu2;
  mu2 = mu * mu;
  a0 = -0.5 * y0 + 1.5 * y1 - 1.5 * y2 + 0.5 * y3;
  a1 = y0 - 2.5 * y1 + 2.0 * y2 - 0.5 * y3;
  a2 = -0.5 * y0 + 0.5 * y2;
  a3 = y1;
  return a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3;
}

/*
   Tension: 1 is high, 0 normal, -1 is low
   Bias: 0 is even,
         positive is towards first segment,
         negative towards the other
*/
double gos_interpolate_hermite(
  double y0,
  double y1,
  double y2,
  double y3,
  double mu,
  double tension,
  double bias) {
  double m0, m1, mu2, mu3;
  double a0, a1, a2, a3;
  mu2 = mu * mu;
  mu3 = mu2 * mu;
  m0 = (y1 - y0) * (1.0 + bias) * (1.0 - tension) / 2.0;
  m0 += (y2 - y1) * (1.0 - bias) * (1.0 - tension) / 2.0;
  m1 = (y2 - y1) * (1.0 + bias) * (1.0 - tension) / 2.0;
  m1 += (y3 - y2) * (1.0 - bias) * (1.0 - tension) / 2.0;
  a0 = 2.0 * mu3 - 3.0 * mu2 + 1.0;
  a1 = mu3 - 2.0 * mu2 + mu;
  a2 = mu3 - mu2;
  a3 = -2.0 * mu3 + 3.0 * mu2;
  return a0 * y1 + a1 * m0 + a2 * m1 + a3 * y2;
}
