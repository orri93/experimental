#include <stdio.h>

#include <SDL2/SDL.h>

#include <modules/mset.h>
#include <modules/mset/texture.h>
#include <modules/mset/handle.h>
#include <modules/mset/draw.h>
#include <modules/mset/loop.h>

static bool gmset_loop_work(gos_mset_context* context, SDL_Event* e);

void gmset_loop_wasm(void* arg) {
  SDL_Event sdlevent;
  gos_mset_context* context = (gos_mset_context*)arg;
  gmset_loop_work(context, &sdlevent);
}

bool gmset_loop(gos_mset_context* context) {
  SDL_Event sdlevent;

  while (context->isgo) {
    if (!gmset_loop_work(context, &sdlevent)) {
      return false;
    }
  }

  return true;
}

bool gmset_loop_work(gos_mset_context* context, SDL_Event* e) {
  if (context->isresize) {
    gmset_texture_free_set(context);
    gmset_texture_create_set(context);
    context->isresize = false;
  }
  if (context->isdraw) {
    gmset_draw(context);
    context->isdraw = false;
  }
  while (SDL_PollEvent(e)) {
    gmset_handle(context, e);
  }

  return true;
}
