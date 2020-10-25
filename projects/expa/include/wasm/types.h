#ifndef GOS_EXPA_WASM_TYPES_H_
#define GOS_EXPA_WASM_TYPES_H_

#include <gos/types.h>

#define GOS_EXPA_WASM_WS_URL 1024

typedef struct gos_performance {
  gos_dynamic_double_array items;
  unsigned long count;
  double value;
  double sum;
  double minimum;
  double maximum;
  double average;
  double sd;
  bool is;
  struct timespec at;
} gos_performance;

typedef struct gos_expa_ws {
  int socket;
  int start_from;
  char url[GOS_EXPA_WASM_WS_URL];
} gos_expa_ws;

#endif
