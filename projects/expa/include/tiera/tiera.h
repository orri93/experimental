#ifndef GOS_TIER_A_H_
#define GOS_TIER_A_H_

#include <stdbool.h>

#include <gos/types.h>

typedef struct gos_tier_a_d {
  double* data;
  double* depth;
  gos_range_1d data_range;
  gos_range_1d depth_range;
  int count;
} gos_tier_a_d;

#ifdef __cplusplus
extern "C" {
#endif

bool gos_tier_a_initialize(
  int toolcount,
  int datapointcount,
  int majorcount);

bool gos_tier_a_generate(
  gos_tier_a_d* data,
  gos_generation_1d* datageneration,
  gos_generation_1d* depthgeneration,
  int major);

gos_tier_a_d* gos_tier_a_get(int tool);

bool gos_tier_a_get_data(double* value, int tool, int index);
bool gos_tier_a_get_depth(double* value, int tool, int index);
bool gos_tier_a_get_data_range(double* from, double* to);
bool gos_tier_a_get_depth_range(double* from, double* to);

const char* gos_tier_a_message();
const char* gos_tier_a_message_length(int* length);

void gos_tier_a_shutdown();

#ifdef __cplusplus
}
#endif

#endif
