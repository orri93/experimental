#ifndef GOS_EXPB_MODULES_MSET_ITEMS_H_
#define GOS_EXPB_MODULES_MSET_ITEMS_H_

#include <stdbool.h>

#include <modules/mset.h>

#ifdef __cplusplus
extern "C" {
#endif

void gmset_items_initalize(gos_mset_context* context);
bool gmset_items_create(gos_mset_context* context);
void gmset_items_shutdown(gos_mset_context* context);

bool gmset_items_create_color_map(gos_mset_context* context);
bool gmset_items_create_sdl(gos_mset_context* context);

void gmset_items_free_color_map(gos_mset_context* context);
void gmset_items_free_sdl(gos_mset_context* context);

#ifdef __cplusplus
}
#endif


#endif
