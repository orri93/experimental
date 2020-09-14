#ifndef GOS_EXP_DRILLING_DENSITY_H_
#define GOS_EXP_DRILLING_DENSITY_H_

#include <stdbool.h>

#include <gos/types.h>

typedef struct gos_drilling_density {
  double* density;
  double* depth;
  int count;
} gos_drilling_density;

#ifdef __cplusplus
extern "C" {
#endif

bool gos_exp_drilling_density_initialize(
  int toolcount,
  int datapointcount,
  int majorcount);

gos_drilling_density* gos_exp_drilling_density_ecd_get(int tool);

bool gos_exp_drilling_density_generate(
  gos_drilling_density* ecd,
  gos_generation_1d* densitygeneration,
  gos_generation_1d* depthgeneration,
  int major);

void gos_exp_drilling_density_shutdown();

const char* gos_exp_drilling_message();
const char* gos_exp_drilling_message_length(int* length);


#ifdef __cplusplus
}
#endif

#endif
