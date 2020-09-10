#include <assert.h>
#include <stdio.h>

#include <gos/experimental/sdl/render.h>

#define GOS_EXP_SDL_RENDER_MESSAGE_SIZE 1024

#ifdef GOS_EXP_SDL_RENDER_LOCK_SURFACE
#define GOS_EXP_SDL_RENDER_SURFACE_LOCKER(s) 
#else

#endif

static char gos_sdl_exp_render_message[GOS_EXP_SDL_RENDER_MESSAGE_SIZE];

static bool gos_sdl_exp_render_lock(GosSdlExpChart* chart);
static void gos_sdl_exp_render_unlock(GosSdlExpChart* chart);

static bool gos_sdl_exp_render_inner(GosSdlExpChart* chart);


bool gos_sdl_exp_render(GosSdlExpChart* chart) {
  bool funres;
#ifdef GOS_EXP_SDL_RENDER_LOCK_SURFACE
  if (funres = gos_sdl_exp_render_lock(chart))
#endif
  {
    if (funres = gos_sdl_exp_render_inner(chart)) {
#ifdef GOS_EXP_SDL_RENDER_LOCK_SURFACE
      if (chart->issurfacelocked) {
        gos_sdl_exp_render_unlock(chart);
      }
#endif
#ifdef GOS_EXP_SDL_RENDER_FLIP
      funres = gos_sdl_exp_render_flip(chart);
#endif
    }
  }
  return funres;
}

bool gos_sdl_exp_render_lock(GosSdlExpChart* chart) {
  int sdlresult;
  assert(chart != NULL);
  assert(chart->surface != NULL);
  if (SDL_MUSTLOCK(chart->surface)) {
    sdlresult = SDL_LockSurface(chart->surface);
    if (sdlresult == 0) {
      chart->issurfacelocked = true;
      return true;
    } else {
      snprintf(
        gos_sdl_exp_render_message,
        GOS_EXP_SDL_RENDER_MESSAGE_SIZE,
        "Failed to locking surface failed with result code %d\n",
        sdlresult);
      return false;
    }
  }
  return true;
}

void gos_sdl_exp_render_unlock(GosSdlExpChart* chart) {
  assert(chart != NULL);
  assert(chart->surface != NULL);
  if (chart->issurfacelocked) {
    SDL_UnlockSurface(chart->surface);
    chart->issurfacelocked = false;
  }
}

bool gos_sdl_exp_render_inner(GosSdlExpChart* chart) {
  int w, h;
  assert(chart != NULL);
  for (w = 0; w < chart->width; w++) {
    for (h = 0; h < chart->height; h++) {

    }
  }
}
