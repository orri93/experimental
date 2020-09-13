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

bool gos_exp_drilling_density_generate(
  gos_drilling_density* ecd,
  gos_generation_1d* densitygeneration,
  gos_generation_1d* depthgeneration,
  int major);

#ifdef __cplusplus
}
#endif

#endif
