#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <gos/dynamic.h>
#include <gos/statistics.h>

#include <wasm/performance.h>

#ifdef GOS_CLOCK_GETTIME
#define GOS_PERFORMANCE_CLOCK CLOCK_REALTIME
#else
#define GOS_PERFORMANCE_TIME_SPEC_BASE TIME_UTC
#endif

#define GOS_PERFORMANCE_GIGA 1000000000.0
#define GOS_PERFORMANCE_MEGA 1000000.0

void gos_performance_initialize(gos_performance* performance) {
  gos_performance_reset(performance);
  (performance->items).items = NULL;
  (performance->items).used = 0;
  (performance->items).size = 0;
}

bool gos_performance_initialize_sd(gos_performance* performance, size_t size) {
  gos_performance_initialize(performance);
  return gos_dynamic_double_array_initialize(&(performance->items), size);
}

void gos_performance_reset(gos_performance* performance) {
  performance->count = 0UL;
  performance->sum = 0.0;
  performance->value = 0.0;
  performance->minimum = DBL_MAX;
  performance->maximum = -DBL_MAX;
  performance->average = 0.0;
  performance->sd = 0.0;
  performance->is = false;
  (performance->at).tv_nsec = 0L;
  (performance->at).tv_sec = 0;
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
    "Average: %.6f, Minimum: %.6f, Maximum: %.6f, Count: %lu",
    performance->average,
    performance->minimum,
    performance->maximum,
    performance->count);
}

int gos_performance_format_sd(gos_performance* performance, char* b, size_t s) {
  return snprintf(
    b, s,
    "Average: %.6f, SD: %.6f, Minimum: %.6f, Maximum: %.6f, Count: %lu",
    performance->average,
    performance->sd,
    performance->minimum,
    performance->maximum,
    performance->count);
}

int gos_performance_start(gos_performance* performance) {
  performance->is = true;
#ifdef GOS_CLOCK_GETTIME
  return clock_gettime(GOS_PERFORMANCE_CLOCK, &(performance->at));
#else
  return timespec_get(&(performance->at), GOS_PERFORMANCE_TIME_SPEC_BASE);
#endif
}

double gos_performance_completed_s(gos_performance* performance) {
  return gos_performance_measure_s(&(performance->at));
}

double gos_performance_completed_ms(gos_performance* performance) {
  return gos_performance_measure_ms(&(performance->at));
}

double gos_performance_measure_s(struct timespec* before) {
  struct timespec at;
  double sd, nsd;
#ifdef GOS_CLOCK_GETTIME
  if (clock_gettime(GOS_PERFORMANCE_CLOCK, &at) == 0) {
    sd = ((double)(at.tv_sec)) - ((double)(before->tv_sec));
    nsd = ((double)(at.tv_nsec)) - ((double)(before->tv_nsec));
    return (GOS_PERFORMANCE_GIGA * sd + nsd) / GOS_PERFORMANCE_GIGA;
  } else {
    return 0.0;
  }
#else
  if (timespec_get(&at, GOS_PERFORMANCE_TIME_SPEC_BASE)) {
    sd = ((double)(at.tv_sec)) - ((double)(before->tv_sec));
    nsd = ((double)(at.tv_nsec)) - ((double)(before->tv_nsec));
    return (GOS_PERFORMANCE_GIGA * sd + nsd) / GOS_PERFORMANCE_GIGA;
  } else {
    return 0.0;
  }
#endif
}

double gos_performance_measure_ms(struct timespec* before) {
  struct timespec at;
  double sd, nsd;
#ifdef GOS_CLOCK_GETTIME
  if (clock_gettime(GOS_PERFORMANCE_CLOCK, &at) == 0) {
    sd = ((double)(at.tv_sec)) - ((double)(before->tv_sec));
    nsd = ((double)(at.tv_nsec)) - ((double)(before->tv_nsec));
    return (GOS_PERFORMANCE_GIGA * sd + nsd) / GOS_PERFORMANCE_MEGA;
  } else {
    return 0.0;
  }
#else
  if (timespec_get(&at, GOS_PERFORMANCE_TIME_SPEC_BASE)) {
    sd = ((double)(at.tv_sec)) - ((double)(before->tv_sec));
    nsd = ((double)(at.tv_nsec)) - ((double)(before->tv_nsec));
    return (GOS_PERFORMANCE_GIGA * sd + nsd) / GOS_PERFORMANCE_MEGA;
  } else {
    return 0.0;
  }
#endif
}

void gos_performance_shutdown(gos_performance* performance) {
  gos_dynamic_double_array_shutdown(&(performance->items));
}
