#include <stdio.h>

#include <SDL2/SDL.h>

#include <modules/mset.h>
#include <modules/mset/texture.h>

#define GOS_MSET_CROSS_SIZE 17

bool gmset_texture_create_set(gos_mset_context* context) {
  SDL_Texture* target;
  int x, y;
  gosmf x0, y0;
  gosmrange* wr;
  gosmrange* hr;
  gosmit i, imax;
  gos_rgb* rgb;

  context->set = SDL_CreateTexture(
    context->renderer,
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_TARGET,
    context->w,
    context->h);
  if (context->set != NULL) {
    imax = context->gradient.count;

    wr = &(context->mrect.a);
    hr = &(context->mrect.b);

    target = SDL_GetRenderTarget(context->renderer);
    SDL_SetRenderTarget(context->renderer, context->set);
    SDL_SetRenderDrawColor(context->renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(context->renderer);

    for (x = 0; x < context->w; x++) {
      for (y = 0; y < context->h; y++) {
        x0 = gos_mandel_map_range(wr, x, context->w);
        y0 = gos_mandel_map_range(hr, y, context->h);
        i = gos_mandel(x0, y0, GOS_MSET_MAX_F, imax);
        rgb = &(context->gradient.gradient[i]);
        SDL_SetRenderDrawColor(
          context->renderer,
          rgb->r,
          rgb->g,
          rgb->b,
          SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(context->renderer, x, y);
      }
    }

    SDL_SetRenderTarget(context->renderer, target);
  } else {
    fprintf(stderr, "Create SDL Texture Failed: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void gmset_texture_free_set(gos_mset_context* context) {
  if (context->set != NULL) {
    SDL_DestroyTexture(context->set);
    context->set = NULL;
  }
}
