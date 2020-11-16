#ifndef GOS_EXPB_MODULES_MSET_TEXTURE_H_
#define GOS_EXPB_MODULES_MSET_TEXTURE_H_

#include <SDL2/SDL.h>

#include <stdbool.h>

#include <modules/mset.h>

#ifdef __cplusplus
extern "C" {
#endif

bool gmset_texture_create_set(gos_mset_context* context);

void gmset_texture_free_set(gos_mset_context* context);

#ifdef __cplusplus
}
#endif

#endif
