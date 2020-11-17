#include <stdio.h>

#include <modules/mset.h>
#include <modules/mset/items.h>
#include <modules/mset/texture.h>

#define GOS_MSET_W 1280
#define GOS_MSET_H 800

#define GOS_MSET_COLOR_STOP_COUNT 6

#define GOS_SDL_INIT_DEFAULT -1
#define GOS_TTF_INIT_DEFAULT -1

void gmset_items_initalize(gos_mset_context* context) {
  context->sdlinit = GOS_SDL_INIT_DEFAULT;
  context->ttfinit = GOS_TTF_INIT_DEFAULT;

  context->w = GOS_MSET_W;
  context->h = GOS_MSET_H;

  context->mrect.a.f = -2.5L;
  context->mrect.a.t = 1.0L;
  context->mrect.b.f = -1.0L;
  context->mrect.b.t = 1.0L;

  context->mcpoint.x = context->mrect.a.f +
    GOS_MSET_HALF * (context->mrect.a.t - context->mrect.a.f);
  context->mcpoint.y = context->mrect.b.f +
    GOS_MSET_HALF * (context->mrect.b.t - context->mrect.b.f);

  context->mr.x = 0;
  context->mr.y = 0;
  context->mr.w = 0;
  context->mr.h = 0;

  context->isgo = true;
  context->isdraw = true;
  context->isdrawrect = false;
  context->isdrawcross = false;
  context->ismouserect = false;
  context->isresize = false;

  context->gstepsize = 64;
  context->colorminit = false;
  context->gradient.gradient = NULL;
  context->gradient.count = 0;

  context->window = NULL;
  context->renderer = NULL;

  context->set = NULL;
}

bool gmset_items_create(gos_mset_context* context) {
  context->colorminit = gmset_items_create_color_map(context);
  if (!context->colorminit) {
    return false;
  }
  return gmset_items_create_sdl(context);
}

void gmset_items_shutdown(gos_mset_context* context) {
  gmset_items_free_color_map(context);
}

bool gmset_items_create_color_map(gos_mset_context* context) {
  int i;
  int* gsizeat;
  int gradientsize[GOS_MSET_COLOR_STOP_COUNT - 1];
  gos_rgb stops[GOS_MSET_COLOR_STOP_COUNT];
  gos_rgb* rgbat = stops;

  gos_color_assign_rgb32(rgbat++, 0x07049b);  /* Dark blue */
  gos_color_assign_rgb32(rgbat++, 0x02f7f3);  /* Cyan */
  gos_color_assign_rgb32(rgbat++, 0x09f725);  /* Green */
  gos_color_assign_rgb32(rgbat++, 0xf4ec04);  /* Yellow */
  gos_color_assign_rgb32(rgbat++, 0xf79d01);  /* Orange */
  gos_color_assign_rgb32(rgbat++, 0x8c0101);  /* Dark red */

  /* Equal sized gradient */
  gsizeat = gradientsize;
  for (i = 0; i < GOS_MSET_COLOR_STOP_COUNT - 1; i++) {
    *(gsizeat++) = context->gstepsize;
  }

  return gos_color_create_rgb_gradient(
    &(context->gradient),
    stops,
    gradientsize,
    GOS_MSET_COLOR_STOP_COUNT,
    GOS_COLOR_GAMMA);
}

bool gmset_items_create_sdl(gos_mset_context* context) {
  context->sdlinit = SDL_Init(SDL_INIT_VIDEO);
  if (context->sdlinit != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return false;
  }

  context->window = SDL_CreateWindow(
    "Hello World!",     /* Title */
    200,                /* X */
    200,                /* Y */
    context->w,         /* W */
    context->h,         /* H */
    SDL_WINDOW_SHOWN);  /* Flags */
  if (context->window == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return false;
  }

  context->renderer = SDL_CreateRenderer(
    context->window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (context->renderer == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return false;
  }

  if (!gmset_texture_create_set(context)) {
    return false;
  }

  return true;
}

void gmset_items_free_color_map(gos_mset_context* context) {
  if (context->colorminit) {
    gos_color_free_rgb_gradient(&(context->gradient));
    context->colorminit = false;
  }
}

void gmset_items_free_sdl(gos_mset_context* context) {
  gmset_texture_free_set(context);

  if (context->renderer != NULL) {
    SDL_DestroyRenderer(context->renderer);
    context->renderer = NULL;
  }

  if (context->window != NULL) {
    SDL_DestroyWindow(context->window);
    context->window = NULL;
  }

  if (context->sdlinit == 0) {
    SDL_Quit();
    context->sdlinit = GOS_SDL_INIT_DEFAULT;
  }
}
