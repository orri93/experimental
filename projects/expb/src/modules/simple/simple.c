#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include <gos/color.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define GOS_SIMPLE_W 640
#define GOS_SIMPLE_H 400

#define GOS_SIMPLE_COLOR_STOP_COUNT 6
#define GOS_SIMPLE_COLOR_STEP_COUNT 36

#define GOS_SDL_INIT_DEFAULT -1

static int sdlinit = GOS_SDL_INIT_DEFAULT;
static gos_rgb_gradient gradient;
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static bool colorminit = false;
static bool isenabled = false;

static bool clear();
static bool initialize();
static bool randommax();
static bool randomn();
static bool loop();
static bool handle(SDL_Event* event, bool* go);
static bool handlekey(SDL_KeyboardEvent* keyevent, bool* go);
static bool createcolormap();
static void eloop(void* arg);
static void shutdown();

#ifdef __EMSCRIPTEN__
int main(int argc, char** argv) {
  int i;
  printf("Starting Simple SDL2 WebAssembly Module\n");
  for (i = 0; i < argc; i++) {
    printf("Argument #%d: %s\n", i, argv[i]);
  }

  if (!initialize()) {
    return EXIT_FAILURE;
  }

  clear();

  emscripten_set_main_loop_arg(eloop, NULL, -1, 1);

  return EXIT_SUCCESS;
}
EMSCRIPTEN_KEEPALIVE void eshutdown() {
  shutdown();
}
EMSCRIPTEN_KEEPALIVE void erandommax() {
  randommax();
}
EMSCRIPTEN_KEEPALIVE void erandom() {
  randomn();
}
EMSCRIPTEN_KEEPALIVE void enable() {
  isenabled = true;
}
EMSCRIPTEN_KEEPALIVE void disabled() {
  isenabled = false;
}
#else
int main(int argc, char** argv) {
  printf("Starting Simple Desktop Module\n");

  if (!initialize()) {
    shutdown();
    return EXIT_FAILURE;
  }

  clear();
  loop();

  shutdown();
  return EXIT_SUCCESS;
}
#endif

bool clear() {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  return true;
}

bool initialize() {
  int createres;

  colorminit = createcolormap();
  if (!colorminit) {
    fprintf(stderr, "Failed to create Color map\n");
    return false;
  }

  sdlinit = SDL_Init(SDL_INIT_VIDEO);
  if (sdlinit != 0) {
    fprintf(stderr, "SDL Initialize Failed: %s\n", SDL_GetError());
    return false;
  }

  createres = SDL_CreateWindowAndRenderer(
    GOS_SIMPLE_W,       /* W */
    GOS_SIMPLE_H,       /* H */
    0,                  /* Window Flags */
    &window,            /* Window */
    &renderer);         /* Renderer */
  if (createres != 0) {
    fprintf(
      stderr,
      "SDL Create Window and Renderer Failed: %s\n",
      SDL_GetError());
    return false;
  }

  return true;
}

bool randommax() {
  gos_rgb* rgb;
  int x, y, m, i, mm;

  mm = GOS_SIMPLE_W * GOS_SIMPLE_H;

  for (x = 0; x < GOS_SIMPLE_W; x++) {
    for (y = 0; y < GOS_SIMPLE_H; y++) {
      m = mm * x * y / gradient.count;
      i = m > 0 ? rand() % m : 0;
      //i = rand() * m / RAND_MAX;
      rgb = &(gradient.gradient[i]);
      SDL_SetRenderDrawColor(
        renderer,
        rgb->r,
        rgb->g,
        rgb->b,
        SDL_ALPHA_OPAQUE);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
  SDL_RenderPresent(renderer);

  return true;
}

bool randomn() {
  gos_rgb* rgb;
  int x, y, i;

  for (x = 0; x < GOS_SIMPLE_W; x++) {
    for (y = 0; y < GOS_SIMPLE_H; y++) {
      //i = rand() * (gradient.count - 1) / RAND_MAX;
      i = rand() % gradient.count;
      rgb = &(gradient.gradient[i]);
      SDL_SetRenderDrawColor(
        renderer,
        rgb->r,
        rgb->g,
        rgb->b,
        SDL_ALPHA_OPAQUE);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
  SDL_RenderPresent(renderer);

  return true;
}

bool loop() {
  bool isgo = true;
  SDL_Event sdl2event;
  while (isgo) {
    while (SDL_PollEvent(&sdl2event)) {
      handle(&sdl2event, &isgo);
    }
  }
  return true;
}

bool handle(SDL_Event* event, bool* go) {
  switch (event->type) {
  case SDL_QUIT:
    if (go) {
      *go = false;
    }
    break;
  case SDL_KEYDOWN:
    if (!handlekey(&(event->key), go)) {
      return false;
    }
    break;
  default:
    /* Do nothing */
    break;
  }
  return true;
}

bool handlekey(SDL_KeyboardEvent* keyevent, bool* go) {
  SDL_Keysym* keysym = &(keyevent->keysym);
  switch (keysym->scancode) {
  case SDL_SCANCODE_Q:
    if (go) {
      *go = false;
    }
    break;
  case SDL_SCANCODE_M:
    randommax();
    break;
  case SDL_SCANCODE_R:
    randomn();
    break;
  default:
    /* Do nothing */
    break;
  }
  return true;
}

bool createcolormap() {
  int i;
  int* gsizeat;
  int gradientsize[GOS_SIMPLE_COLOR_STOP_COUNT - 1];
  gos_rgb stops[GOS_SIMPLE_COLOR_STOP_COUNT];
  gos_rgb* rgbat = stops;

  gos_color_assign_rgb32(rgbat++, 0x07049b);  /* Dark blue */
  gos_color_assign_rgb32(rgbat++, 0x02f7f3);  /* Cyan */
  gos_color_assign_rgb32(rgbat++, 0x09f725);  /* Green */
  gos_color_assign_rgb32(rgbat++, 0xf4ec04);  /* Yellow */
  gos_color_assign_rgb32(rgbat++, 0xf79d01);  /* Orange */
  gos_color_assign_rgb32(rgbat++, 0x8c0101);  /* Dark red */

  /* Equal sized gradient */
  gsizeat = gradientsize;
  for (i = 0; i < GOS_SIMPLE_COLOR_STOP_COUNT - 1; i++) {
    *(gsizeat++) = GOS_SIMPLE_COLOR_STEP_COUNT;
  }

  return gos_color_create_rgb_gradient(
    &gradient,
    stops,
    gradientsize,
    GOS_SIMPLE_COLOR_STOP_COUNT,
    GOS_COLOR_GAMMA);
}

void eloop(void* arg) {
  SDL_Event sdl2event;
  if (isenabled) {
    while (SDL_PollEvent(&sdl2event)) {
      handle(&sdl2event, NULL);
    }
  }
}

void shutdown() {
  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }

  if (window != NULL) {
    SDL_DestroyWindow(window);
    window = NULL;
  }

  if (sdlinit == 0) {
    SDL_Quit();
    sdlinit = GOS_SDL_INIT_DEFAULT;
  }

  if (colorminit) {
    gos_color_free_rgb_gradient(&gradient);
    colorminit = false;
  }
}
