#ifndef GOS_EXPB_MODULES_MSET_MOUSE_H_
#define GOS_EXPB_MODULES_MSET_MOUSE_H_

#include <stdbool.h>

#include <SDL2/SDL.h>

#include <modules/mset.h>

#ifdef __cplusplus
extern "C" {
#endif

void gmet_mouse_start(gos_mset_context* context, SDL_MouseButtonEvent* button);
void gmet_mouse_motion(gos_mset_context* context, SDL_MouseMotionEvent* motion);
void gmet_mouse_completed(gos_mset_context* context, SDL_MouseButtonEvent* button);

#ifdef __cplusplus
}
#endif

#endif
