#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "statistic.h"

#define INITIAL_STATISTIC_ARRAY_SIZE 8

static double statistic_ss(Statistic* statistic);

void statistic_init(Statistic* statistic) {
  statistic->count = 0;
  statistic->capacity = INITIAL_STATISTIC_ARRAY_SIZE;
  statistic->sum = 0.0;
  statistic->min = DBL_MAX;
  statistic->max = -DBL_MAX;
  statistic->array = (double*)malloc(INITIAL_STATISTIC_ARRAY_SIZE * sizeof(double));
}

void statistic_free(Statistic* statistic) {
  if (statistic->array) {
    free(statistic->array);
    statistic->array = 0;
  }
}

void statistic_reset(Statistic* statistic) {
  statistic_free(statistic);
  statistic_init(statistic);
}

void statistic_add(Statistic* statistic, double value) {
  if (value > statistic->max) {
    statistic->max = value;
  }
  if (value < statistic->min) {
    statistic->min = value;
  }
  if (statistic->count == statistic->capacity) {
    statistic->capacity *= 2;
    statistic->array = (double*)realloc(statistic->array, statistic->capacity * sizeof(double));
  }
  statistic->array[statistic->count] = value;
  statistic->count++;
  statistic->sum += value;
}

double statistic_mean(Statistic* statistic) {
  return statistic->sum / statistic->count;
}

double statistic_sd(Statistic* statistic) {
  double ss;
  ss = statistic_ss(statistic);
  return sqrt(ss / statistic->count);
}

double statistic_cssd(Statistic* statistic) {
  double ss;
  ss = statistic_ss(statistic);
  return sqrt(ss / (statistic->count - 1));
}

double statistic_ss(Statistic* statistic) {
  int i;
  double mean, ss;

  ss = 0.0;
  mean = statistic_mean(statistic);

  for (i = 0; i < statistic->count; i++) {
    ss += (statistic->array[i] - mean) * (statistic->array[i] - mean);
  }

  return ss;
}
