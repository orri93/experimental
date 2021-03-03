#include <stdint.h>
#include <stdlib.h>

#include <SDL.h>

#include <gos/noise.h>

#include <modules/hm/hm.h>
#include <modules/hm/hmdemo.h>

static bool ghmd_loop_work(wd3hm* context, wd3hmdemo* demo, SDL_Event* event);
static bool ghmd_handle(wd3hm* context, wd3hmdemo* demo, SDL_Event* event);
static bool ghmd_handle_keyboard(
  wd3hm* context,
  wd3hmdemo* demo,
  SDL_KeyboardEvent* event);
static bool ghmd_handle_key_state(
  wd3hm* context,
  wd3hmdemo* demo,
  Uint8* state);
static bool ghmd_next(wd3hm* context, wd3hmdemo* demo);

void ghmd_draw_pattern(wd3hm* context) {
  int i, j, k;
  gos_rgb* rgb;
  Uint32 pixel;
  for (i = 0; i < context->screen.width; i++) {
    for (j = 0; j < context->screen.height; j++) {
      // k = gos_noise_random_maximum_integer(context->gradient.count);
      k = i * j % context->gradient.count;
      rgb = &(context->gradient.gradient[k]);
      pixel = SDL_MapRGB(context->surface->format, rgb->r, rgb->g, rgb->b);
      ghm_draw_pixel(context, i, j, pixel);
    }
  }
}

void ghmd_initialize(wd3hmdemo* demo) {
  demo->type = 0;
  demo->isgo = false;
  demo->isrun = false;
  demo->data = NULL;
}

bool ghmd_create(wd3hm* context, wd3hmdemo* demo, int type, int count) {
  int i, j;
  double n, y;
  demo->type = type;
  context->count = count;
  if (context->count > 0) {
    if (ghm_create_data(context)) {
      if (ghm_create_buffer(context)) {
        if (ghm_create_separation(context)) {
          demo->data = malloc(context->count * sizeof(gos_point_2d));
          if (demo->data != NULL) {
            y = 5.0 + gos_noise_white(GOS_NOISE_DEFAULT_SEED, 0, 0);
            for (j = 0; j < context->count; j++) {
              n = 1.0 + (double)j;
              demo->data[j].x = 100.0 * n;
              demo->data[j].y = y;
              y += gos_noise_white(GOS_NOISE_DEFAULT_SEED, 0, j) + 10.0 * n;
            }

            for (i = 0; i < context->screen.width; i++) {
              for (j = 0; j < context->count; j++) {
                ghm_data_set(context, j, demo->data[j].x, demo->data[j].y);
                demo->data[j].x += 0.1;
                demo->data[j].y +=
                  gos_noise_white(GOS_NOISE_DEFAULT_SEED, i, j) / 10.0;
              }
              ghm_buffer_insert(context, i);
            }
            ghm_rescale(context);
            return true;
          } else {
            fprintf(stderr, "Failed to allocate memory for the demo data\n\n");
          }
        }
      }
    }
  }
  return false;
}

bool ghmd_render(wd3hm* context, wd3hmdemo* demo) {
  if (ghm_lock(context)) {
    switch (demo->type) {
    case WD3_HM_DEMO_TYPE_PATTERN:
      ghmd_draw_pattern(context);
      break;
    case WD3_HM_DEMO_TYPE_ECD:
      ghm_render_all(context);
      ghm_render_separation(context);
      break;
    }
    ghm_flip(context);
    ghm_unlock(context);
    return true;
  } else {
    return false;
  }
}

bool ghmd_loop(wd3hm* context, wd3hmdemo* demo) {
  SDL_Event sdlevent;
  demo->isgo = true;
  while (demo->isgo) {
    if (!ghmd_loop_work(context, demo, &sdlevent)) {
      return false;
    }
  }
  return true;
}

void ghmd_shutdown(wd3hmdemo* demo) {
  if (demo->data) {
    free(demo->data);
    demo->data = NULL;
  }
}


bool ghmd_loop_work(wd3hm* context, wd3hmdemo* demo, SDL_Event* event) {
  Uint8* keystate = SDL_GetKeyState(NULL);
  if (!ghmd_handle_key_state(context, demo, keystate)) {
    return false;
  }
  while (SDL_PollEvent(event)) {
    if (!ghmd_handle(context, demo, event)) {
      return false;
    }
  }
  if (demo->isrun) {
    ghmd_next(context, demo);
  }
  return true;
}

bool ghmd_handle(wd3hm* context, wd3hmdemo* demo, SDL_Event* event) {
  switch (event->type) {
  case SDL_QUIT:
    demo->isgo = false;
    break;
  case SDL_KEYDOWN:
    if (!ghmd_handle_keyboard(context, demo, &(event->key))) {
      return false;
    }
    break;
  default:
    /* Do nothing */
    break;
  }
  return true;
}

bool ghmd_handle_keyboard(
  wd3hm* context,
  wd3hmdemo* demo,
  SDL_KeyboardEvent* event) {
  SDL_keysym* keysym = &(event->keysym);
  switch (keysym->sym) {
  case SDLK_q:
    demo->isgo = false;
    break;
  case SDLK_s:
    demo->isrun = demo->isrun ? false : true;
    break;
  case SDLK_n:
    // ghmd_next(context, demo);
    break;
  default:
    /* Do nothing */
    break;
  }
  return true;
}

bool ghmd_handle_key_state(wd3hm* context, wd3hmdemo* demo, Uint8* state) {
  if (state[SDLK_n]) {
    ghmd_next(context, demo);
  }
  return true;
}


bool ghmd_next(wd3hm* context, wd3hmdemo* demo) {
  int i, k;
  gos_rgb* rgb;
  Uint32 pixel;
  ghm_lock(context);
  switch (demo->type) {
  case WD3_HM_DEMO_TYPE_PATTERN:
    ghm_shift(context);
    for (i = 0; i < context->screen.height; i++) {
      k = gos_noise_random_maximum_integer(context->gradient.count - 1);
      rgb = &(context->gradient.gradient[k]);
      pixel = SDL_MapRGB(context->surface->format, rgb->r, rgb->g, rgb->b);
      ghm_draw_pixel(context, context->screen.width - 1, i, pixel);
    }
    break;
  case WD3_HM_DEMO_TYPE_ECD:
    ghm_buffer_shift(context);
    for (i = 0; i < context->count; i++) {
      ghm_data_set(context, i, demo->data[i].x, demo->data[i].y);
      demo->data[i].x += 0.1;
      demo->data[i].y +=
        gos_noise_white(GOS_NOISE_DEFAULT_SEED, i, i) / 10.0;
    }
    ghm_buffer_insert_last(context);
    ghm_rescale(context);
    ghm_render_all(context);
    ghm_render_separation(context);
    break;
  }
  // if (!ghm_draw(context)) { return false; }
  ghm_flip(context);
  ghm_unlock(context);
  return true;
}
