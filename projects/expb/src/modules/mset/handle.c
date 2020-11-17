#include <stdio.h>

#include <SDL2/SDL.h>

#include <modules/mset.h>
#include <modules/mset/move.h>
#include <modules/mset/scale.h>
#include <modules/mset/mouse.h>
#include <modules/mset/handle.h>

static void gmset_handle_keyboard(
  gos_mset_context* context,
  SDL_KeyboardEvent* event);

bool gmset_handle(gos_mset_context* context, SDL_Event* event) {
  switch (event->type) {
  case SDL_QUIT:
    context->isgo = false;
    break;
  case SDL_MOUSEMOTION:
    if (context->ismouserect) {
      gmet_mouse_motion(context, &(event->motion));
    }
    break;
  case SDL_MOUSEBUTTONDOWN:
    gmet_mouse_start(context, &(event->button));
    break;
  case SDL_MOUSEBUTTONUP:
    if (context->ismouserect) {
      gmet_mouse_completed(context, &(event->button));
    }
    break;
  case SDL_KEYDOWN:
    gmset_handle_keyboard(context, &(event->key));
    break;
  }
  return true;
}

void gmset_handle_keyboard(gos_mset_context* context, SDL_KeyboardEvent* event) {
  gosmpoint point;
  SDL_Keysym* keysym = &(event->keysym);
  switch (keysym->scancode) {
  case SDL_SCANCODE_Q:
    context->isgo = false;
    break;
  case SDL_SCANCODE_M:
    if (keysym->mod & KMOD_SHIFT) {
      printf("Large magnify\n");
      gmset_scale_resize(context, GOS_MSET_RESIZE_RATIO / 2.0L);
    } else {
      printf("Magnify\n");
      gmset_scale_resize(context, GOS_MSET_RESIZE_RATIO);
    }
    break;
  case SDL_SCANCODE_UP:
    gmset_move_cross_up(context, GOS_MSET_STEP);
    break;
  case SDL_SCANCODE_DOWN:
    gmset_move_cross_down(context, GOS_MSET_STEP);
    break;
  case SDL_SCANCODE_LEFT:
    gmset_move_cross_left(context, GOS_MSET_STEP);
    break;
  case SDL_SCANCODE_RIGHT:
    gmset_move_cross_right(context, GOS_MSET_STEP);
    break;
  default:
    /* Do nothing */
    break;
  }
}
