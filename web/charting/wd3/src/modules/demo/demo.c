/*
 * Emscripten WebAssembly SDL Demo based on Emscripten Tutorial Example see
 * https://emscripten.org/docs/getting_started/Tutorial.html
 *
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#ifdef WD3_USE_SDL_MAIN
#include <SDL_main.h>
#endif
#endif

#include <gos/types.h>
#include <gos/text.h>
#include <gos/color.h>
#include <gos/screen.h>

#define DEMO_SDL_INIT_DEFAULT     -1
#define DEMO_SDL_BPP              32
#define DEMO_DEFAULT_WIDTH       256
#define DEMO_DEFAULT_HEIGHT      256
#define DEMO_COLOR_STOP_COUNT      5
#define DEMO_COLOR_STOP_SIZE      64

typedef struct demot {
  int sdlinit;
  bool isgo;  /* Only used by the desktop demo */
  bool iscreate;
  gos_screen screen;
  gos_rgb_gradient gradient;
  SDL_Surface* surface;
} demot;

static demot context;
static void demo_initialize(demot* context, int width, int height);
static void demo_parse_argument(demot* context, int argc, char** argv);
static bool demo_create(demot* context);
static bool demo_create_demo(demot* context);
static bool demo_create_surface(demot* context);
static bool demo_create_gradient(demot* context, int size);
static void demo_create_pattern_a(demot* context);
static void demo_create_pattern_b(demot* context);
static void demo_draw_pixel(demot* context, int x, int y, Uint32 pixel);
static bool demo_lock(demot* context);
static bool demo_flip(demot* context);
#ifndef __EMSCRIPTEN__
static bool demo_loop(demot* context);
static bool demo_loop_work(demot* context, SDL_Event* event);
static bool demo_handle(demot* context, SDL_Event* event);
static bool demo_handle_keyboard(demot* context, SDL_KeyboardEvent* event);
#endif
static void demo_unlock(demot* context);
static void demo_shutdown(demot* context);

#ifdef __EMSCRIPTEN__

int main(int argc, char** argv) {
  printf("Initialize the Emscripten SDL Demo\n");
  demo_initialize(&context, DEMO_DEFAULT_WIDTH, DEMO_DEFAULT_HEIGHT);
  demo_parse_argument(&context, argc, argv);
  if (context.iscreate) {
    if (demo_create(&context)) {
      return demo_create_demo(&context) ? EXIT_SUCCESS : EXIT_FAILURE;
    } else {
      return EXIT_FAILURE;
    }
  } else {
    return EXIT_SUCCESS;
  }
}

EMSCRIPTEN_KEEPALIVE void shutdown() {
  printf("Shutting down the Emscripten SDL Demo\n");
  demo_shutdown(&context);
}

EMSCRIPTEN_KEEPALIVE void create(int width, int height) {
  printf("Create the SDL Demo to %d x %d\n", width, height);
  context.screen.width = width;
  context.screen.height = height;
  if (demo_create_surface(&context)) {
    demo_create_demo(&context);
  }
}

EMSCRIPTEN_KEEPALIVE void resize(int width, int height) {
  // printf("Resizing the SDL Demo to %d x %d\n", width, height);
  context.screen.width = width;
  context.screen.height = height;
  if (demo_create_surface(&context)) {
    demo_create_demo(&context);
  }
}

#else

#ifdef WD3_USE_SDL_MAIN
int SDL_main(int argc, char** argv) {
  printf("Initialize the SDL Demo with SDL Main\n");
#else
int main(int argc, char** argv) {
  printf("Initialize the SDL Demo\n");
#endif
  demo_initialize(&context, DEMO_DEFAULT_WIDTH, DEMO_DEFAULT_HEIGHT);
  demo_parse_argument(&context, argc, argv);
  if (demo_create(&context)) {
    if (demo_create_demo(&context)) {
      if (demo_loop(&context)) {
        demo_shutdown(&context);
        return EXIT_SUCCESS;
      }
    }
  }
  demo_shutdown(&context);
  return EXIT_FAILURE;
}

#endif

void demo_initialize(demot* context, int width, int height) {
  context->sdlinit = DEMO_SDL_INIT_DEFAULT;
  context->isgo = false;
  context->iscreate = true;
  context->screen.width = width;
  context->screen.height = height;
  context->surface = NULL;
}

void demo_parse_argument(demot* context, int argc, char** argv) {
  if (argc > 1) {
    if (gos_text_are_all_char_digits(argv[1])) {
      context->screen.width = atoi(argv[1]);
      printf("Setting screen width to %d\n", context->screen.width);
    }
  }
  if (argc > 2) {
    if (gos_text_are_all_char_digits(argv[2])) {
      context->screen.height = atoi(argv[2]);
      printf("Setting screen height to %d\n", context->screen.height);
    }
  }
  if (argc > 3) {
    if (gos_text_are_all_char_digits(argv[3])) {
      context->iscreate = atoi(argv[3]) > 0;
      if (context->iscreate) {
        printf("Setting is create to true\n");
      } else {
        printf("Setting is create to false\n");
      }
    }
  }
}

bool demo_create(demot* context) {
  context->sdlinit = SDL_Init(SDL_INIT_VIDEO);
  if (context->sdlinit != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return false;
  }
  if (demo_create_surface(context)) {
    return demo_create_gradient(context, DEMO_COLOR_STOP_SIZE);
  } else {
    return false;
  }
}

bool demo_create_surface(demot* context) {
  context->surface = SDL_SetVideoMode(
    context->screen.width,    /* Width */
    context->screen.height,   /* Heigh */
    DEMO_SDL_BPP,             /* Bits per pixel */
    SDL_SWSURFACE);           /* Flags */
  if (context->surface != NULL) {
    return true;
  } else {
    fprintf(stderr, "SDL_SetVideoMode Error: %s\n", SDL_GetError());
    return false;
  }
}

bool demo_create_demo(demot* context) {
  if (demo_lock(context)) {
    if (context->screen.height <= 256 && context->screen.width <= 256) {
      demo_create_pattern_a(context);
    } else {
      demo_create_pattern_b(context);
    }
    demo_unlock(context);
    return demo_flip(context);
  } else {
    return false;
  }
}

bool demo_create_gradient(demot* context, int size) {
  int i;
  int* gsizeat;
  int gsize[DEMO_COLOR_STOP_COUNT - 1];
  gos_rgb rgbs[DEMO_COLOR_STOP_COUNT];
  gos_rgb* rgbat = rgbs;

  gos_color_assign_rgb32(rgbat++, 0x07049b);  /* Dark blue */
  gos_color_assign_rgb32(rgbat++, 0x02f7f3);  /* Cyan */
  gos_color_assign_rgb32(rgbat++, 0x09f725);  /* Green */
  gos_color_assign_rgb32(rgbat++, 0x8c0101);  /* Dark red */
  gos_color_assign_rgb32(rgbat++, 0xf4ec04);  /* Yellow */

  /* Equal sized gradient */
  gsizeat = gsize;
  for (i = 0; i < DEMO_COLOR_STOP_COUNT - 1; i++) {
    *(gsizeat++) = size;
  }

  return gos_color_create_rgb_gradient(
    &context->gradient,
    rgbs,
    gsize,
    DEMO_COLOR_STOP_COUNT,
    GOS_COLOR_GAMMA);
}

void demo_create_pattern_a(demot* context) {
  int i, j, a;
  Uint32 pixel;
  for (i = 0; i < context->screen.width; i++) {
    for (j = 0; j < context->screen.height; j++) {
      a = (i + j) % 255;
      pixel = SDL_MapRGBA(context->surface->format, i, j, 255 - i, a);
      demo_draw_pixel(context, i, j, pixel);
    }
  }
}

void demo_create_pattern_b(demot* context) {
  int i, j, k;
  gos_rgb* rgb;
  Uint32 pixel;
  for (i = 0; i < context->screen.width; i++) {
    for (j = 0; j < context->screen.height; j++) {
      k = i * j % context->gradient.count;
      rgb = &(context->gradient.gradient[k]);
      pixel = SDL_MapRGB(context->surface->format, rgb->r, rgb->g, rgb->b);
      demo_draw_pixel(context, i, j, pixel);
    }
  }
}

void demo_draw_pixel(demot* context, int x, int y, Uint32 pixel) {
  *(((Uint32*)(context->surface->pixels)) +
    gos_screen_index(&context->screen, x, y)) = pixel;
}

bool demo_lock(demot* context) {
  if (SDL_MUSTLOCK(context->surface)) {
    if (SDL_LockSurface(context->surface) != 0) {
      fprintf(stderr, "SDL_LockSurface Error: %s\n\n", SDL_GetError());
      return false;
    }
  }
  return true;
}

bool demo_flip(demot* context) {
  if (SDL_Flip(context->surface) != 0) {
    fprintf(stderr, "SDL_Flip Error: %s\n\n", SDL_GetError());
    return false;
  }
  return true;
}

#ifndef __EMSCRIPTEN__
bool demo_loop(demot* context) {
  SDL_Event sdlevent;
  context->isgo = true;
  while (context->isgo) {
    if (!demo_loop_work(context, &sdlevent)) {
      return false;
    }
  }
  return true;
}

bool demo_loop_work(demot* context, SDL_Event * event) {
  Uint8* keystate = SDL_GetKeyState(NULL);
  while (SDL_PollEvent(event)) {
    if (!demo_handle(context, event)) {
      return false;
    }
  }
  return true;
}

bool demo_handle(demot* context, SDL_Event * event) {
  switch (event->type) {
  case SDL_QUIT:
    context->isgo = false;
    break;
  case SDL_KEYDOWN:
    if (!demo_handle_keyboard(context, &(event->key))) {
      return false;
    }
    break;
  default:
    /* Do nothing */
    break;
  }
  return true;
}

bool demo_handle_keyboard(demot* context, SDL_KeyboardEvent * event) {
  SDL_keysym* keysym = &(event->keysym);
  switch (keysym->sym) {
  case SDLK_q:
    context->isgo = false;
    break;
  default:
    /* Do nothing */
    break;
  }
  return true;
}
#endif

void demo_unlock(demot* context) {
  if (SDL_MUSTLOCK(context->surface)) {
    SDL_UnlockSurface(context->surface);
  }
}

void demo_shutdown(demot* context) {
  gos_color_free_rgb_gradient(&context->gradient);
  if (context->sdlinit == 0) {
    SDL_Quit();
    context->sdlinit = DEMO_SDL_INIT_DEFAULT;
  }
}
