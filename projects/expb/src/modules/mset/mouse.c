#include <stdio.h>

#include <SDL2/SDL.h>

#include <modules/mset/mouse.h>
#include <modules/mset/map.h>

static void gmet_mouse_update(gos_mset_context* context, int x, int y);

void gmet_mouse_start(gos_mset_context* context, SDL_MouseButtonEvent* button) {
  int mx, my;

  mx = button->x;
  my = button->y;

  if (mx >= 0 && my >= 0 && mx < context->w && my < context->h) {
    context->ismouserect = true;
    context->mr.x = mx;
    context->mr.y = my;
  }
}

void gmet_mouse_motion(gos_mset_context* context, SDL_MouseMotionEvent* motion) {
  gmet_mouse_update(context, motion->x, motion->y);
  context->isdrawrect = true;
  context->isdraw = true;
}

void gmet_mouse_completed(gos_mset_context* context, SDL_MouseButtonEvent* button) {
  gmet_mouse_update(context, button->x, button->y);

  gosmpoint f, t;

  if (context->mr.w > 0 && context->mr.h > 0) {
    gmset_map_point_from_screen(
      &f,
      &(context->mrect),
      context->mr.x,
      context->mr.y,
      context->w,
      context->h);
    gmset_map_point_from_screen(
      &t,
      &(context->mrect),
      context->mr.x + context->mr.w,
      context->mr.y + context->mr.h,
      context->w,
      context->h);
    context->mrect.a.f = f.x;
    context->mrect.a.t = t.x;
    context->mrect.b.f = f.y;
    context->mrect.b.t = t.y;
    context->isresize = true;
    context->isdraw = true;
  } else {
    context->isdraw = false;
  }

  context->ismouserect = false;
  context->isdrawrect = false;
}

void gmet_mouse_update(gos_mset_context* context, int x, int y) {
  context->mr.w = x - context->mr.x;
  context->mr.h = y - context->mr.y;
}
