#include <stdio.h>

#include <SDL2/SDL.h>

#include <modules/mset.h>
#include <modules/mset/map.h>
#include <modules/mset/draw.h>
#include <modules/mset/texture.h>

#define GOS_MSET_CROSS_SIZE 17

static bool gmset_draw_cross(gos_mset_context* context);
static bool gmset_draw_rect(gos_mset_context* context);

bool gmset_draw(gos_mset_context* context) {
  /* SDL_Rect crossrect; */

  SDL_RenderClear(context->renderer);

  SDL_RenderCopy(context->renderer, context->set, NULL, NULL);

  if (context->isdrawcross) {
    gmset_draw_cross(context);
  } else if (context->isdrawrect) {
    gmset_draw_rect(context);
  }

  SDL_RenderPresent(context->renderer);

  return true;
}

bool gmset_draw_cross(gos_mset_context* context) {
  int x, y;
  int hs = GOS_MSET_CROSS_SIZE / 2;
  gmset_map_point_to_screen(
    &(context->mrect),
    &(context->mcpoint),
    &x,
    &y,
    context->w,
    context->h);

  SDL_SetRenderDrawColor(context->renderer, 0xff, 0x10, 0x20, 0x40);

  SDL_RenderDrawLine(context->renderer, x - hs, y, x + hs, y);
  SDL_RenderDrawLine(context->renderer, x, y - hs, x, y + hs);

  return true;
}

bool gmset_draw_rect(gos_mset_context* context) {
  SDL_SetRenderDrawColor(context->renderer, 0xff, 0x10, 0x20, 0x40);
  SDL_RenderDrawRect(context->renderer, &(context->mr));

  return true;
}
