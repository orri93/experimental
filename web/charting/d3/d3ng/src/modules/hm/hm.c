#include <stdlib.h>
#include <stdio.h>

#include <gos/text.h>
#include <gos/noise.h>

#include <modules/hm.h>

#define GOS_SDL_INIT_DEFAULT         -1
#define GOS_CHART_COLOR_STOP_COUNT    6

static bool ghm_create_gradient(gos_rgb_gradient* gradient);
static bool ghm_create_sdl(goshm* context, int x, int y, const char* title);
static bool ghm_shift(goshm* context);
static bool ghm_loop_work(goshm* context, SDL_Event* event);
static bool ghm_handle(goshm* context, SDL_Event* event);
static bool ghm_handle_keyboard(goshm* context, SDL_KeyboardEvent* event);
static void ghm_shutdown_sdl(goshm* context);

void ghm_initialize(goshm* context, int width, int height) {
  context->sdlinit = GOS_SDL_INIT_DEFAULT;

  context->isgo = false;

  context->width = width;
  context->height = height;

  /* gradient */
  context->gradient.gradient = NULL;
  context->gradient.count = 0;

  context->window = NULL;
  context->renderer = NULL;

  context->texture = NULL;
}

void ghm_parse_argument(goshm* context, int argc, char** argv) {
  if (argc > 1) {
    if (gos_text_are_all_char_digits(argv[1])) {
      context->width = atoi(argv[1]);
      printf("Setting width to %d\n", context->width);
    }
  }
  if (argc > 2) {
    if (gos_text_are_all_char_digits(argv[2])) {
      context->height = atoi(argv[2]);
      printf("Setting height to %d\n", context->height);
    }
  }
}

bool ghm_create(goshm* context, int x, int y, const char* title) {
  if (!ghm_create_gradient(&context->gradient)) {
    ghm_shutdown(context);
    return false;
  }
  if (!ghm_create_sdl(context, x, y, title)) {
    ghm_shutdown(context);
    return false;
  }
  return true;
}

bool ghm_create_texture(goshm* context) {
  int i, j, k;
  gos_rgb* rgb;
  SDL_Texture* target;
  context->texture = SDL_CreateTexture(
    context->renderer,
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_TARGET,
    context->width,
    context->height);
  if (context->texture != NULL) {
    target = SDL_GetRenderTarget(context->renderer);
    if (SDL_SetRenderTarget(context->renderer, context->texture) != 0) {
      fprintf(stderr, "SDL_SetRenderTarget Error: %s\n\n", SDL_GetError());
      return false;
    }
    for (i = 0; i < context->width; i++) {
      for (j = 0; j < context->height; j++) {
        k = gos_noise_random_maximum_integer(context->gradient.count);
        rgb = &(context->gradient.gradient[k]);
        if (SDL_SetRenderDrawColor(
          context->renderer,
          rgb->r,
          rgb->g,
          rgb->b,
          SDL_ALPHA_OPAQUE) != 0) {
          fprintf(
            stderr,
            "SDL_SetRenderDrawColor Error: %s\n",
            SDL_GetError());
          return false;
        }
        if (SDL_RenderDrawPoint(context->renderer, i, j) != 0) {
          fprintf(stderr, "SDL_RenderDrawPoint Error: %s\n\n", SDL_GetError());
          return false;
        }
      }
    }
    if (SDL_SetRenderTarget(context->renderer, target) != 0) {
      fprintf(stderr, "SDL_SetRenderTarget Error: %s\n\n", SDL_GetError());
      return false;
    }
  } else {
    fprintf(stderr, "Create SDL Texture Failed: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

bool ghm_draw(goshm* context) {
  if (SDL_RenderClear(context->renderer) != 0) {
    fprintf(stderr, "SDL_RenderClear Error: %s\n\n", SDL_GetError());
    return false;
  }
  if (SDL_RenderCopy(context->renderer, context->texture, NULL, NULL) != 0) {
    fprintf(stderr, "SDL_RenderCopy Error: %s\n\n", SDL_GetError());
    return false;
  }
  SDL_RenderPresent(context->renderer);
  return true;
}

bool ghm_loop(goshm* context) {
  SDL_Event sdlevent;

  context->isgo = true;
  while (context->isgo) {
    if (!ghm_loop_work(context, &sdlevent)) {
      return false;
    }
  }

  return true;
}

void ghm_shutdown(goshm* context) {
  gos_color_free_rgb_gradient(&context->gradient);
  ghm_shutdown_sdl(context);
}

bool ghm_create_gradient(gos_rgb_gradient* gradient) {
  int i;
  int* gsizeat;
  int gradientsize[GOS_CHART_COLOR_STOP_COUNT - 1];
  gos_rgb stops[GOS_CHART_COLOR_STOP_COUNT];
  gos_rgb* rgbat = stops;

  gos_color_assign_rgb32(rgbat++, 0x07049b);  /* Dark blue */
  gos_color_assign_rgb32(rgbat++, 0x02f7f3);  /* Cyan */
  gos_color_assign_rgb32(rgbat++, 0x09f725);  /* Green */
  gos_color_assign_rgb32(rgbat++, 0xf4ec04);  /* Yellow */
  gos_color_assign_rgb32(rgbat++, 0xf79d01);  /* Orange */
  gos_color_assign_rgb32(rgbat++, 0x8c0101);  /* Dark red */

  /* Equal sized gradient */
  gsizeat = gradientsize;
  for (i = 0; i < GOS_CHART_COLOR_STOP_COUNT - 1; i++) {
    *(gsizeat++) = 64;
  }

  return gos_color_create_rgb_gradient(
    gradient,
    stops,
    gradientsize,
    GOS_CHART_COLOR_STOP_COUNT,
    GOS_COLOR_GAMMA);
}

bool ghm_create_sdl(goshm* context, int x, int y, const char* title) {
  context->sdlinit = SDL_Init(SDL_INIT_VIDEO);
  if (context->sdlinit != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return false;
  }

  context->window = SDL_CreateWindow(
    title,              /* Title */
    x,                  /* X */
    y,                  /* Y */
    context->width,     /* W */
    context->height,    /* H */
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

  return true;
}

bool ghm_shift(goshm* context) {
  int i, j, k;
  gos_rgb* rgb;
  SDL_Rect srcrect, dstrect;
  SDL_Texture* target;
  SDL_Texture* previous = context->texture;
  context->texture = SDL_CreateTexture(
    context->renderer,
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_TARGET,
    context->width,
    context->height);
  if (context->texture != NULL) {
    target = SDL_GetRenderTarget(context->renderer);
    if (SDL_SetRenderTarget(context->renderer, context->texture) != 0) {
      fprintf(stderr, "SDL_SetRenderTarget Error: %s\n\n", SDL_GetError());
      return false;
    }

    srcrect.x = 1;
    dstrect.x = 0;
    srcrect.y = dstrect.y = 0;
    srcrect.w = dstrect.w = context->width - 1;
    srcrect.h = dstrect.h = context->height;

    if (SDL_RenderCopy(context->renderer, previous, &srcrect, &dstrect) != 0) {
      fprintf(stderr, "SDL_RenderCopy Error: %s\n\n", SDL_GetError());
      return false;
    }
    i = context->width - 1;
    for (j = 0; j < context->height; j++) {
      k = gos_noise_random_maximum_integer(context->gradient.count);
      rgb = &(context->gradient.gradient[k]);
      if (SDL_SetRenderDrawColor(
        context->renderer,
        rgb->r,
        rgb->g,
        rgb->b,
        SDL_ALPHA_OPAQUE) != 0) {
        fprintf(
          stderr,
          "SDL_SetRenderDrawColor Error: %s\n",
          SDL_GetError());
        return false;
      }
      if (SDL_RenderDrawPoint(context->renderer, i, j) != 0) {
        fprintf(stderr, "SDL_RenderDrawPoint Error: %s\n\n", SDL_GetError());
        return false;
      }
    }

    if (SDL_SetRenderTarget(context->renderer, target) != 0) {
      fprintf(stderr, "SDL_SetRenderTarget Error: %s\n\n", SDL_GetError());
      return false;
    }
  } else {
    fprintf(stderr, "Create SDL Texture Failed: %s\n", SDL_GetError());
    return false;
  }

  SDL_DestroyTexture(previous);

  return true;
}

bool ghm_loop_work(goshm* context, SDL_Event* event) {
  while (SDL_PollEvent(event)) {
    if (!ghm_handle(context, event)) {
      return false;
    }
  }
  return true;
}

bool ghm_handle(goshm* context, SDL_Event* event) {
  switch (event->type) {
  case SDL_QUIT:
    context->isgo = false;
    break;
  case SDL_KEYDOWN:
    if (!ghm_handle_keyboard(context, &(event->key))) {
      return false;
    }
    break;
  }
  return true;
}

bool ghm_handle_keyboard(goshm* context, SDL_KeyboardEvent* event) {
  SDL_Keysym* keysym = &(event->keysym);
  switch (keysym->scancode) {
  case SDL_SCANCODE_Q:
    context->isgo = false;
    break;
  case SDL_SCANCODE_N:
    if (ghm_shift(context)) {
      if (!ghm_draw(context)) {
        return false;
      }
    } else {
      return false;
    }
    break;
  }
  return true;
}

void ghm_shutdown_sdl(goshm* context) {
  if (context->texture != NULL) {
    SDL_DestroyTexture(context->texture);
    context->texture = NULL;
  }
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
