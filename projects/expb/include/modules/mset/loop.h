#ifndef GOS_EXPB_MODULES_MSET_LOOP_H_
#define GOS_EXPB_MODULES_MSET_LOOP_H_

#include <stdbool.h>

#include <modules/mset.h>

#ifdef __cplusplus
extern "C" {
#endif

void gmset_loop_wasm(void* arg);
bool gmset_loop(gos_mset_context* context);

#ifdef __cplusplus
}
#endif


#endif
