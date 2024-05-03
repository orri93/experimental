#ifndef _STATISTIC_H_
#define _STATISTIC_H_

#include <stddef.h>

typedef struct {
  size_t count;
  size_t capacity;
  double sum;
  double min;
  double max;
  double* array;
} Statistic;

void statistic_init(Statistic* statistic);
void statistic_free(Statistic* statistic);
void statistic_reset(Statistic* statistic);
void statistic_add(Statistic* statistic, double value);
double statistic_mean(Statistic* statistic);
double statistic_sd(Statistic* statistic);
double statistic_cssd(Statistic* statistic);

#endif /* _STATISTIC_H_ */
