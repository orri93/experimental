#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#include <gos/dynamic.h>
#include <gos/statistics.h>

#include <wasm/performance.h>

#define GOS_PERFORMANCE_TIME_SPEC_BASE TIME_UTC
#define GOS_PERFORMANCE_NANO_SEC 1000000000.0

void gos_performance_initialize(gos_performance* performance) {
  performance->count = 0UL;
  performance->sum = 0.0;
  performance->value = 0.0;
  performance->minimum = DBL_MAX;
  performance->maximum = -DBL_MAX;
  performance->average = 0.0;
  performance->sd = 0.0;
  performance->is = false;
  performance->withsd = false;
  (performance->items).items = NULL;
  (performance->items).used = 0;
  (performance->items).size = 0;
  (performance->at).tv_nsec = 0L;
  (performance->at).tv_sec = 0;
}

bool gos_performance_initialize_sd(gos_performance* performance, size_t size) {
  gos_performance_initialize(performance);
  return gos_dynamic_double_array_initialize(&(performance->items), size);
}

void gos_performance_reset(gos_performance* performance) {
  gos_performance_initialize(performance);
}

void gos_performance_reset_sd(gos_performance* performance) {
  gos_performance_reset(performance);
  (performance->items).used = 0;
}

void gos_performance_update(gos_performance* performance, double value) {
  performance->value = value;
  performance->sum += value;
  performance->count++;
  performance->minimum =
    value < performance->minimum ? value : performance->minimum;
  performance->maximum =
    value > performance->maximum ? value : performance->maximum;
}

bool gos_performance_update_sd(gos_performance* performance, double value) {
  if (gos_dynamic_double_array_add(&(performance->items), value)) {
    gos_performance_update(performance, value);
    return true;
  } else {
    return false;
  }
}

void gos_performance_calculate(gos_performance* performance) {
  performance->average = performance->sum / (double)(performance->count);
}

void gos_performance_calculate_sd(gos_performance* performance) {
  gos_performance_calculate(performance);
  performance->sd = gos_statistics_sd(
    (performance->items).items,
    performance->average,
    (performance->items).used);
}

int gos_performance_format(gos_performance* performance, char* b, size_t s) {
  return snprintf(
    b, s,
    "Average: %f.6, Minimum: %f.6, Maximum: %f.6, Count: %lu",
    performance->average,
    performance->minimum,
    performance->maximum,
    performance->count);
}

int gos_performance_format_sd(gos_performance* performance, char* b, size_t s) {
  return snprintf(
    b, s,
    "Average: %f.6, SD: %f.6, Minimum: %f.6, Maximum: %f.6, Count: %lu",
    performance->average,
    performance->sd,
    performance->minimum,
    performance->maximum,
    performance->count);
}

int gos_performance_start(gos_performance* performance) {
  performance->is = true;
  return timespec_get(&(performance->at), GOS_PERFORMANCE_TIME_SPEC_BASE);
}

double gos_performance_completed(gos_performance* performance) {
  return gos_performance_measure(&(performance->at));
}

double gos_performance_measure(struct timespec* before) {
  struct timespec at;
  double sd, nsd;
  if (timespec_get(&at, GOS_PERFORMANCE_TIME_SPEC_BASE)) {
    sd = ((double)(at.tv_sec)) - ((double)(before->tv_sec));
    nsd = ((double)(at.tv_nsec)) - ((double)(before->tv_nsec));
    return (GOS_PERFORMANCE_NANO_SEC * sd + nsd) / GOS_PERFORMANCE_NANO_SEC;
  } else {
    return 0.0;
  }
}

void gos_performance_shutdown(gos_performance* performance) {
  gos_dynamic_double_array_shutdown(&(performance->items));
}
