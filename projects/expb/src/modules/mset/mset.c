#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include <gos/color.h>

#include <modules/mc/mandel.h>

#define GOS_MSET_W 1280
#define GOS_MSET_H 800

#define GOS_MSET_HALF 0.5L

#define GOS_MSET_COLOR_STOP_COUNT 6
#define GOS_MSET_MAX_F 6.0L

#define GOS_SDL_INIT_DEFAULT -1
#define GOS_TTF_INIT_DEFAULT -1

static int sdlinit = GOS_SDL_INIT_DEFAULT;
static int ttfinit = GOS_TTF_INIT_DEFAULT;
static bool isdraw = true;
static bool colorminit = false;
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static gosmrect mrect = { { -2.5L, 1.0L }, { -1.0L, 1.0L } };

static gos_rgb_gradient gradient;

static void magnratio(gosmpoint* point, gosmf r);
static void keyboard(SDL_KeyboardEvent* event, bool* go);
static void handle(SDL_Event* event, bool* go);
static void draw(gosmrect* rect);
static bool colormap();
static void shutdown();

int main(int argc, char** argv) {
  SDL_Event sdlevent;
  bool go;

  colorminit = colormap();
  if (!colorminit) {
    fprintf(stderr, "Failed to create the color map\n");
    return EXIT_FAILURE;
  }

  sdlinit = SDL_Init(SDL_INIT_VIDEO);
  if (sdlinit != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow(
    "Hello World!",     /* Title */
    200,                /* X */
    200,                /* Y */
    GOS_MSET_W,         /* W */
    GOS_MSET_H,         /* H */
    SDL_WINDOW_SHOWN);  /* Flags */
  if (window == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    shutdown();
    return EXIT_FAILURE;
  }

  renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    shutdown();
    return EXIT_FAILURE;
  }

  go = true;
  while (go) {
    if (isdraw) {
      draw(&mrect);
      SDL_RenderPresent(renderer);
      isdraw = false;
    }
    while (SDL_PollEvent(&sdlevent)) {
      handle(&sdlevent, &go);
    }
  }

  shutdown();
  return EXIT_SUCCESS;
}

void magnratio(gosmpoint* point, gosmf r) {
  gosmf wd, hd;
  wd = mrect.a.t - mrect.a.f;
  hd = mrect.b.t - mrect.b.f;
  wd *= GOS_MSET_HALF * r;
  hd *= GOS_MSET_HALF * r;
  mrect.a.f = point->x - wd;
  mrect.a.t = point->x + wd;
  mrect.b.f = point->y - hd;
  mrect.b.t = point->y + hd;
}

void keyboard(SDL_KeyboardEvent* event, bool* go) {
  gosmpoint point;
  SDL_Keysym* keysym = &(event->keysym);
  switch (keysym->sym) {
  case SDLK_q:
    *go = false;
    break;
  case SDLK_m:
    point.x = mrect.a.f + GOS_MSET_HALF * (mrect.a.t - mrect.a.f);
    point.y = mrect.b.f + GOS_MSET_HALF * (mrect.b.t - mrect.b.f);
    magnratio(&point, 0.9L);
    isdraw = true;
    break;
  case SDLK_UP:
    break;
  }
}

void handle(SDL_Event* event, bool* go) {
  switch (event->type) {
  case SDL_QUIT:
    *go = false;
    break;
  case SDL_KEYDOWN:
    keyboard(&(event->key), go);
    break;
  }
}

void draw(gosmrect* rect) {
  int w, h, x, y;
  gosmf x0, y0;
  gosmrange* wr;
  gosmrange* hr;
  gosmit i, imax;
  gos_rgb* rgb;

  imax = gradient.count;

  SDL_GetWindowSize(window, &w, &h);

  wr = &(rect->a);
  hr = &(rect->b);

  for (x = 0; x < w; x++) {
    for (y = 0; y < h; y++) {
      x0 = gos_mandel_map_range(wr, x, w);
      y0 = gos_mandel_map_range(hr, y, h);
      i = gos_mandel(x0, y0, GOS_MSET_MAX_F, imax);
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
}

bool colormap() {
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
    *(gsizeat++) = 64;
  }

  return gos_color_create_rgb_gradient(
    &gradient,
    stops,
    gradientsize,
    GOS_MSET_COLOR_STOP_COUNT,
    GOS_COLOR_GAMMA);
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
