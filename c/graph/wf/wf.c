#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TWO_PI 2.0 * M_PI

static double next_x(double x, double n, double t);
static double next_y(double x, double n, double t);

int main(int argc, char* argv[]) {
  double x, y, t;
  int i, n;

  x = 0.0;
  y = 0.0;

  if (argc > 2) {
    n = atoi(argv[1]);
    t = atof(argv[2]);
    if (argc > 4) {
      x = atof(argv[3]);
      y = atof(argv[4]);
    }
  } else {
    fprintf(stderr, "usage: wf n t [x y]");
    return EXIT_FAILURE;
  }

  printf("%lf\t%lf\n", x, y);
  for (i = 0; i <= n; i++) {
    x = next_x(x, i, t);
    y = next_y(y, i ,t);
    printf("%lf\t%lf\n", x, y);
  }

  return EXIT_SUCCESS;
}

double next_x(double x, double n, double t) {
  return x + (pow(n, 1.5 / (n + 1000.0))) * sin(TWO_PI * t * (
    cos(t * 100.0) *
    sin(n * 0.05 + t * 30.0) *
    10.0 + t * 10.0) + 1.8 * n * t);
}

double next_y(double x, double n, double t) {
  return x + (pow(n, 1.5 / (n + 1000.0))) * cos(TWO_PI * t * (
    cos(t * 100.0) *
    sin(n * 0.05 + t * 30.0) *
    10.0 + t * 10.0) + 1.8 * n * t);
}
