#include <stdio.h>

#include <SDL2/SDL.h>

#include <modules/mset.h>
#include <modules/mset/texture.h>
#include <modules/mset/handle.h>
#include <modules/mset/draw.h>
#include <modules/mset/loop.h>

bool gmset_loop(gos_mset_context* context) {
  SDL_Event sdlevent;

  while (context->isgo) {
    if (context->isresize) {
      gmset_texture_free_set(context);
      gmset_texture_create_set(context);
      context->isresize = false;
    }
    if (context->isdraw) {
      gmset_draw(context);
      context->isdraw = false;
    }
    while (SDL_PollEvent(&sdlevent)) {
      gmset_handle(context, &sdlevent);
    }
  }

  return true;
}
