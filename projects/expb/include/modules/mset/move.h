#ifndef GOS_EXPB_MODULES_MSET_MOVE_H_
#define GOS_EXPB_MODULES_MSET_MOVE_H_

#include <stdbool.h>

#include <modules/mset.h>

#ifdef __cplusplus
extern "C" {
#endif

void gmset_move_cross_up(gos_mset_context* context, gosmf r);
void gmset_move_cross_down(gos_mset_context* context, gosmf r);
void gmset_move_cross_left(gos_mset_context* context, gosmf r);
void gmset_move_cross_right(gos_mset_context* context, gosmf r);

#ifdef __cplusplus
}
#endif

#endif
