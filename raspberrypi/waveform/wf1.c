/*
 * Raspberry Pi C waveform experiment 1
 *
 * Compile:
 *   gcc -Wall -pthread -o wf1 wf1.c -lm
 *
 * Usage:
 *   ./wf1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

#define VALUE_MAXIMUM   0xfff

#define VALUE_COUNT      1024
#define X_STEP           0.05
#define X_MAXIMUM        16.0 * M_PI

int main(int argc, char *argv[]) {
  int c, i;
  double x, y, y1, y2, y3, ymin, ymax;
  double ys[VALUE_COUNT];
  uint16_t values[VALUE_COUNT];

  x = 0.0;
  ymin = DBL_MAX;
  ymax = -DBL_MAX;
  for (c = 0; c < VALUE_COUNT; c++) {
    y1 = sin(x);
    y2 = sin(4.0 * 2.333 * x);
    y3 = sin(4.0 * 4.666 * x);
    y = y1 + y2 + y3;
    if (y < ymin) {
      ymin = y;
    }
    if (y > ymax) {
      ymax = y;
    }
    ys[c] = y;
    x += X_STEP;
    if (x > X_MAXIMUM) {
      break;
    }
  }

  for (i = 0; i < c; i++) {
    values[i] = (uint16_t) (VALUE_MAXIMUM * (ys[i] - ymin) / (ymax - ymin));
  }

  return EXIT_SUCCESS;
}
