#ifndef GOS_EXPB_MODULES_MSET_HANDLE_H_
#define GOS_EXPB_MODULES_MSET_HANDLE_H_

#include <stdbool.h>

#include <SDL2/SDL.h>

#include <modules/mset.h>

#ifdef __cplusplus
extern "C" {
#endif

bool gmset_handle(gos_mset_context* context, SDL_Event* event);

#ifdef __cplusplus
}
#endif


#endif
