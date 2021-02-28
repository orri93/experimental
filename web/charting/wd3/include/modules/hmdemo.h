#ifndef WD3_HM_DEMO_C_MD_HM_H_
#define WD3_HM_DEMO_C_MD_HM_H_

#include <stdbool.h>

#include <gos/types.h>

#include <modules/hmtype.h>

#define WD3_HM_DEMO_TYPE_PATTERN  1
#define WD3_HM_DEMO_TYPE_ECD      2

typedef struct wd3hmdemo {
  int type;
  gos_point_2d* data;
} wd3hmdemo;

#ifdef __cplusplus
extern "C" {
#endif

void ghmd_draw_pattern(wd3hm* context);
void ghmd_initialize(wd3hmdemo* demo);
bool ghmd_create(wd3hm* context, wd3hmdemo* demo, int type, int count);
bool ghmd_render(wd3hm* context, wd3hmdemo* demo);
bool ghmd_loop(wd3hm* context, wd3hmdemo* demo);
void ghmd_shutdown(wd3hmdemo* demo);

#ifdef __cplusplus
}
#endif

#endif
