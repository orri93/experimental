#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include <gos/text.h>
#include <gos/color.h>
#include <gos/scale.h>
#include <gos/screen.h>
#include <gos/interpolate.h>

#include <modules/hm.h>

#define WD3_SDL_INIT_DEFAULT         -1
#define WD3_SDL_BPP                  32
#define WD3_CHART_COLOR_STOP_COUNT    6

static bool ghm_create_default_gradient(wd3hm* context);
static bool ghm_create_sdl(wd3hm* context);
static void ghm_swap(wd3hm* a, wd3hm* b);
static int ghm_data_index(wd3hm* context, double value);
static double ghm_data_interpolate(gos_point_2d* p0, gos_point_2d* p1, double x);
static double ghm_data_determination(wd3hm* context, int index, double value);
static void ghm_shutdown_sdl(wd3hm* context);

void ghm_initialize(wd3hm* context, int width, int height) {
  context->sdlinit = WD3_SDL_INIT_DEFAULT;

  context->screen.width = width;
  context->screen.height = height;

  /* gradient */
  context->gradient.gradient = NULL;
  context->gradient.count = 0;

  context->surface = NULL;

  context->yscale.domain.from = 0.0;
  context->yscale.domain.to = 1.0;
  context->yscale.range.from = 0.0;
  context->yscale.range.to = 0.0;

  context->zscale.domain.from = 0.0;
  context->zscale.domain.to = 1.0;
  context->zscale.range.from = 0.0;
  context->zscale.range.to = 0.0;

  /* Colors */
  context->stops = WD3_CHART_COLOR_STOP_COUNT;
  context->undefined = 0;
  context->separation = NULL;
  context->stop = NULL;
  context->size = NULL;

  /* Data */
  context->count = 0;
  context->data = NULL;
  context->buffer = NULL;
}

void ghm_parse_argument(wd3hm* context, int argc, char** argv) {
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
      context->count = atoi(argv[3]);
      printf("Setting count to %d\n", context->count);
    }
  }
}

bool ghm_create(wd3hm* context) {
  // Set either y scale range from or to parameter to height
  context->yscale.range.to = (double)(context->screen.height) - 1.0;
  if (!ghm_create_buffer(context)) {
    ghm_shutdown(context);
    return false;
  }
  if (!ghm_create_data(context)) {
    ghm_shutdown(context);
    return false;
  }
  if (!ghm_create_stops(context)) {
    ghm_shutdown(context);
    return false;
  }
  if (!ghm_create_default_gradient(context)) {
    ghm_shutdown(context);
    return false;
  }
  context->zscale.range.to = (double)(context->gradient.count) - 1.0;
  if (!ghm_create_sdl(context)) {
    ghm_shutdown(context);
    return false;
  }
  if (!ghm_create_separation(context)) {
    ghm_shutdown(context);
    return false;
  }
  return true;
}

bool ghm_create_data(wd3hm* context) {
  if (context->count > 0) {
    context->data = malloc(context->count * sizeof(gos_point_2d));
    if (context->data == NULL) {
      fprintf(stderr, "Failed to allocate memory for the data\n");
      return false;
    }
  }
  return true;
}

bool ghm_create_buffer(wd3hm* context) {
  int i;
  if (context->count > 0) {
    context->buffer = calloc(context->count, sizeof(gos_point_2d*));
    if (context->buffer != NULL) {
      for (i = 0; i < context->count; i++) {
        context->buffer[i] = malloc(
          context->screen.width * sizeof(gos_point_2d));
        if (context->buffer[i] == NULL) {
          fprintf(stderr, "Failed to allocate memory for the buffer data\n");
          return false;
        }
      }
    } else {
      return(stderr, "Failed to allocate memory for the buffer\n");
      return false;
    }
  }
  return true;
}

bool ghm_create_separation(wd3hm* context) {
  int i;
  Uint32 pixel;
  if (context->count > 0) {
    context->separation = malloc(context->count * sizeof(Uint32));
    if (context->separation) {
      pixel = SDL_MapRGB(context->surface->format, 0xff, 0xff, 0xff);
      for (i = 0; i < context->count; i++) {
        context->separation[i] = pixel;
      }
    } else {
      fprintf(stderr, "Failed to allocate memory for the separation\n");
      return false;
    }
  }
  return true;
}

bool ghm_create_gradient(wd3hm* context) {
  bool result;
  result = gos_color_create_rgb_gradient(
    &context->gradient,
    context->stop,
    context->size,
    context->stops,
    GOS_COLOR_GAMMA);
  context->zscale.range.to = (double)(context->gradient.count) - 1.0;
  return result;
}

bool ghm_create_stops(wd3hm* context) {
  if (context->stops > 0) {
    context->stop = malloc(context->stops * sizeof(gos_rgb));
    if (context->stop == NULL) {
      fprintf(stderr, "Failed to allocate memory for the color stops\n");
      return false;
    }
    context->size = malloc((context->stops - 1) * sizeof(int));
    if (context->size == NULL) {
      fprintf(stderr, "Failed to allocate memory for the color sizes\n");
      return false;
    }
  }
  return true;
}

bool ghm_change_stops(wd3hm* context, int count, bool copy) {
  int i;
  wd3hm* pcontext, tcontext;
  if (copy) {
    pcontext = &tcontext;
    ghm_initialize(pcontext, context->screen.width, context->screen.height);
  } else {
    ghm_shutdown_stops(context);
    pcontext = context;
  }
  pcontext->stops = count;
  if (!ghm_create_stops(pcontext)) {
    ghm_shutdown_stops(pcontext);
    pcontext->stops = 0;
    return false;
  }
  if (copy) {
    for (i = 0; i < context->stops && i < pcontext->stops; i++) {
      pcontext->stop[i] = context->stop[i];
      if (i < context->stops - 1 && i < pcontext->stops - 1) {
        pcontext->size[i] = context->size[i];
      }
    }
    ghm_swap(context, pcontext);
    ghm_shutdown_stops(pcontext);
  }
  return true;
}

bool ghm_change(wd3hm* context, int count, bool copy) {
  int i;
  wd3hm* pcontext, tcontext;
  if (copy) {
    pcontext = &tcontext;
    ghm_initialize(pcontext, context->screen.width, context->screen.height);
  } else {
    ghm_shutdown_data(context);
    ghm_shutdown_buffer(context);
    ghm_shutdown_separation(context);
    pcontext = context;
  }
  pcontext->count = count;
  if (!ghm_create_data(pcontext)) {
    pcontext->count = 0;
    return false;
  }
  if (!ghm_create_buffer(pcontext)) {
    ghm_shutdown_data(pcontext);
    pcontext->count = 0;
    return false;
  }
  if (!ghm_create_separation(pcontext)) {
    ghm_shutdown_data(pcontext);
    ghm_shutdown_buffer(pcontext);
    pcontext->count = 0;
    return false;
  }
  if (copy) {
    for (i = 0; i < context->count && i < pcontext->count; i++) {
      pcontext->data[i] = context->data[i];
      pcontext->separation[i] = context->separation[i];
      memcpy(
        pcontext->buffer[i],
        context->buffer[i],
        context->screen.width * sizeof(gos_point_2d));
    }
    ghm_swap(context, pcontext);
    ghm_shutdown_data(pcontext);
    ghm_shutdown_buffer(pcontext);
    ghm_shutdown_separation(pcontext);
  }
  return true;
}

bool ghm_data_set(wd3hm* context, int index, double x, double y) {
  if (index >= 0) {
    if (index < context->count) {
      (context->data[index]).x = x;
      (context->data[index]).y = y;
      return true;
    } else {
      fprintf(stderr, "The index value %d is over the count %d\n",
        index,
        context->count);
      return false;
    }
  } else {
    fprintf(stderr, "The index value %d is less than zero\n", index);
    return false;
  }
}

bool ghm_data_check(wd3hm* context) {
  int i;
  for (i = 1; i < context->count; i++) {
    if ((context->data[i]).x < (context->data[i - 1]).x) {
      return false;
    }
    if ((context->data[i]).y < (context->data[i - 1]).y) {
      return false;
    }
  }
  return true;
}

bool ghm_set_stop(wd3hm* context, int index, const char* color) {
  gos_rgb rgb;
  if (gos_color_assign_text(&rgb, color)) {
    memcpy(context->stop + index, &rgb, sizeof(gos_rgb));
    return true;
  } else {
    return false;
  }
}

void ghm_set_size(wd3hm* context, int index, int size) {
  int i;
  if (index >= 0) {
    context->size[index] = size;
  } else {
    for (i = 0; i < context->stops - 1; i++) {
      context->size[i] = size;
    }
  }
}

double ghm_point_determination(wd3hm* context, double value) {
  double y = gos_scale_reverse(&context->yscale, value);
  int i = ghm_data_index(context, y);
  return i > 0 ? ghm_data_determination(context, i, y) : NAN;
}

void ghm_buffer_insert(wd3hm* context, int at) {
  int i = 0;
  for (i = 0; i < context->count; i++) {
    context->buffer[i][at] = context->data[i];
  }
}

void ghm_buffer_insert_last(wd3hm* context) {
  ghm_buffer_insert(context, context->screen.width - 1);
}

void ghm_buffer_shift(wd3hm* context) {
  int i;
  for (i = 0; i < context->count; i++) {
    memmove(
      context->buffer[i],
      context->buffer[i] + 1,
      (((size_t)(context->screen.width)) - 1) * sizeof(gos_point_2d));
  }
}

void ghm_buffer_set(wd3hm* context, int at) {
  int i;
  for (i = 0; i < context->count; i++) {
    context->data[i].x = context->buffer[i][at].x;
    context->data[i].y = context->buffer[i][at].y;
  }
}

void ghm_rescale(wd3hm* context) {
  int i, j;
  context->yscale.domain.from = DBL_MAX;
  context->yscale.domain.to = -DBL_MAX;
  context->zscale.domain.from = DBL_MAX;
  context->zscale.domain.to = -DBL_MAX;
  for (i = 0; i < context->screen.width; i++) {
    for (j = 0; j < context->count; j++) {
      if (context->buffer[j][i].x < context->yscale.domain.from) {
        context->yscale.domain.from = context->buffer[j][i].x;
      }
      if (context->buffer[j][i].x > context->yscale.domain.to) {
        context->yscale.domain.to = context->buffer[j][i].x;
      }
      if (context->buffer[j][i].y < context->zscale.domain.from) {
        context->zscale.domain.from = context->buffer[j][i].y;
      }
      if (context->buffer[j][i].y > context->zscale.domain.to) {
        context->zscale.domain.to = context->buffer[j][i].y;
      }
    }
  }
}

double ghm_domain_get(wd3hm* context, int flag) {
  switch (flag & WD3_HM_DOMAIN_TYPE_MASK) {
  case WD3_HM_DOMAIN_TYPE_Y:
    switch (flag & WD3_HM_DOMAIN_MASK) {
    case WD3_HM_DOMAIN_FROM:
      return context->yscale.domain.from;
    case WD3_HM_DOMAIN_TO:
      return context->yscale.domain.to;
    default:
      return NAN;
    }
  case WD3_HM_DOMAIN_TYPE_Z:
    switch (flag & WD3_HM_DOMAIN_MASK) {
    case WD3_HM_DOMAIN_FROM:
      return context->zscale.domain.from;
    case WD3_HM_DOMAIN_TO:
      return context->zscale.domain.to;
    default:
      return NAN;
    }
  default:
    return NAN;
  }
}

void ghm_domain_set(wd3hm* context, int flag, double value) {
  switch (flag & WD3_HM_DOMAIN_TYPE_MASK) {
  case WD3_HM_DOMAIN_TYPE_Y:
    switch (flag & WD3_HM_DOMAIN_MASK) {
    case WD3_HM_DOMAIN_FROM:
      context->yscale.domain.from = value;
      break;
    case WD3_HM_DOMAIN_TO:
      context->yscale.domain.to = value;
      break;
    default:
      /* Do nothing */
      break;
    }
  case WD3_HM_DOMAIN_TYPE_Z:
    switch (flag & WD3_HM_DOMAIN_MASK) {
    case WD3_HM_DOMAIN_FROM:
      context->zscale.domain.from = value;
      break;
    case WD3_HM_DOMAIN_TO:
      context->zscale.domain.to = value;
      break;
    default:
      /* Do nothing */
      break;
    }
  default:
    /* Do nothing */
    break;
  }
}

void ghm_domain_y_set(wd3hm* context, double from, double to) {
  context->yscale.domain.from = from;
  context->yscale.domain.to = to;
}

void ghm_domain_z_set(wd3hm* context, double from, double to) {
  context->zscale.domain.from = from;
  context->zscale.domain.to = to;
}

void ghm_draw_pixel(wd3hm* context, int x, int y, Uint32 pixel) {
  *(((Uint32*)(context->surface->pixels)) +
    gos_screen_index(&context->screen, x, y)) = pixel;
}

bool ghm_set_undefined(wd3hm* context, const char* color) {
  gos_rgb rgb;
  if (gos_color_assign_text(&rgb, color)) {
    context->undefined = SDL_MapRGB(
      context->surface->format,
      rgb.r,
      rgb.g,
      rgb.b);
    return true;
  } else {
    return false;
  }
}

bool ghm_set_separation(wd3hm* context, int index, const char* color) {
  gos_rgb rgb;
  if (gos_color_assign_text(&rgb, color)) {
    context->separation[index] = SDL_MapRGB(
      context->surface->format,
      rgb.r,
      rgb.g,
      rgb.b);
    return true;
  } else {
    return false;
  }
}

bool ghm_set_all_separation(wd3hm* context, const char* color) {
  int i;
  gos_rgb rgb;
  Uint32 pixel;
  if (gos_color_assign_text(&rgb, color)) {
    pixel = SDL_MapRGB(
      context->surface->format,
      rgb.r,
      rgb.g,
      rgb.b);
    for (i = 0; i < context->count; i++) {
      context->separation[i] = pixel;
    }
    return true;
  } else {
    return false;
  }
}

void ghm_render(wd3hm* context, int x) {
  int i, k;
  double z;
  gos_rgb* rgb;
  Uint32 pixel;
  for (i = 0; i < context->screen.height; i++) {
    z = ghm_point_determination(context, (double)i);
    if (isfinite(z)) {
      k = (int)gos_scale_value(&context->zscale, z);
      assert(k >= 0);
      assert(k < context->gradient.count);
      rgb = &(context->gradient.gradient[k]);
      pixel = SDL_MapRGB(context->surface->format, rgb->r, rgb->g, rgb->b);
    } else {
      pixel = context->undefined;
    }
    ghm_draw_pixel(context, x, i, pixel);
  }
}

void ghm_render_last(wd3hm* context) {
  ghm_render(context, context->screen.width - 1);
}

void ghm_render_all(wd3hm* context) {
  int i;
  for (i = 0; i < context->screen.width; i++) {
    ghm_buffer_set(context, i);
    ghm_render(context, i);
  }
}

void ghm_render_separation(wd3hm* context) {
  double value;
  int i, j, y;
  Uint32 pixel;
  for (i = 0; i < context->count; i++) {
    pixel = context->separation[i];
    for (j = 0; j < context->screen.width; j++) {
      value = context->buffer[i][j].x;
      y = (int)gos_scale_value(&context->yscale, value);
      ghm_draw_pixel(context, j, y, pixel);
    }
  }
}

bool ghm_lock(wd3hm* context) {
  if (SDL_MUSTLOCK(context->surface)) {
    if (SDL_LockSurface(context->surface) != 0) {
      fprintf(stderr, "SDL_LockSurface Error: %s\n\n", SDL_GetError());
      return false;
    }
  }
  return true;
}

bool ghm_flip(wd3hm* context) {
  if (SDL_Flip(context->surface) != 0) {
    fprintf(stderr, "SDL_Flip Error: %s\n\n", SDL_GetError());
    return false;
  }
  return true;
}

void ghm_unlock(wd3hm* context) {
  if (SDL_MUSTLOCK(context->surface)) {
    SDL_UnlockSurface(context->surface);
  }
}

void ghm_shift(wd3hm* context) {
  int i;
  Uint32* pixels = (Uint32*)(context->surface->pixels);
  for (i = 0; i < context->screen.height; i++) {
    memmove(
      pixels,
      pixels + 1,
      (((size_t)(context->screen.width)) - 1) * sizeof(Uint32));
    pixels += context->screen.width;
  }
}

void ghm_shutdown(wd3hm* context) {
  ghm_shutdown_sdl(context);
  ghm_shutdown_gradient(context);
  ghm_shutdown_separation(context);
  ghm_shutdown_stops(context);
  ghm_shutdown_data(context);
  ghm_shutdown_buffer(context);
}

void ghm_shutdown_data(wd3hm* context) {
  if (context->data != NULL) {
    free(context->data);
    context->data = NULL;
  }
}

void ghm_shutdown_buffer(wd3hm* context) {
  int i;
  if (context->buffer != NULL) {
    for (i = 0; i < context->count; i++) {
      if (context->buffer[i]) {
        free(context->buffer[i]);
      }
    }
    free(context->buffer);
    context->buffer = NULL;
  }
}

void ghm_shutdown_separation(wd3hm* context) {
  if (context->separation != NULL) {
    free(context->separation);
    context->separation = NULL;
  }
}

void ghm_shutdown_gradient(wd3hm* context) {
  gos_color_free_rgb_gradient(&context->gradient);
}

void ghm_shutdown_stops(wd3hm* context) {
  if (context->stop != NULL) {
    free(context->stop);
    context->stop = NULL;
  }
  if (context->size != NULL) {
    free(context->size);
    context->size = NULL;
  }
}

bool ghm_create_default_gradient(wd3hm* context) {
  int i;
  int* gsizeat;
  gos_rgb* rgbat = context->stop;

  gos_color_assign_rgb32(rgbat++, 0x07049b);  /* Dark blue */
  gos_color_assign_rgb32(rgbat++, 0x02f7f3);  /* Cyan */
  gos_color_assign_rgb32(rgbat++, 0x09f725);  /* Green */
  gos_color_assign_rgb32(rgbat++, 0xf4ec04);  /* Yellow */
  gos_color_assign_rgb32(rgbat++, 0xf79d01);  /* Orange */
  gos_color_assign_rgb32(rgbat++, 0x8c0101);  /* Dark red */

  /* Equal sized gradient */
  gsizeat = context->size;
  for (i = 0; i < WD3_CHART_COLOR_STOP_COUNT - 1; i++) {
    *(gsizeat++) = 64;
  }

  return ghm_create_gradient(context);
}

bool ghm_create_sdl(wd3hm* context) {
  context->sdlinit = SDL_Init(SDL_INIT_VIDEO);
  if (context->sdlinit != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return false;
  }
  context->surface = SDL_SetVideoMode(
    context->screen.width,    /* Width */
    context->screen.height,   /* Heigh */
    WD3_SDL_BPP,              /* Bits per pixel */
    SDL_SWSURFACE);           /* Flags */
  if (context->surface == NULL) {
    fprintf(stderr, "SDL_SetVideoMode Error: %s\n", SDL_GetError());
    return false;
  }
  return true;
}

void ghm_swap(wd3hm* a, wd3hm* b) {
  wd3hm t;
  t.stop = a->stop;
  t.size = a->size;
  t.data = a->data;
  t.buffer = a->buffer;
  t.separation = a->separation;
  a->stop = b->stop;
  a->size = b->size;
  a->data = b->data;
  a->buffer = b->buffer;
  a->separation = b->separation;
  b->stop = t.stop;
  b->size = t.size;
  b->data = t.data;
  b->buffer = t.buffer;
  b->separation = t.separation;
}

int ghm_data_index(wd3hm* context, double value) {
  int i;
  for (i = 1; i < context->count; i++) {
    if (value >= context->data[i - 1].x && value < context->data[i].x) {
      return i;
    }
  }
  return -1;
}

double ghm_data_interpolate(gos_point_2d* p0, gos_point_2d* p1, double x) {
  double mu = (x - p0->x) / (p1->x - p0->x);
  return gos_interpolate_linear(p0->y, p1->y, mu);
}

double ghm_data_determination(wd3hm* context, int index, double value) {
  return ghm_data_interpolate(
    context->data + index - 1,
    context->data + index,
    value);
}

void ghm_shutdown_sdl(wd3hm* context) {
  if (context->sdlinit == 0) {
    SDL_Quit();
    context->sdlinit = WD3_SDL_INIT_DEFAULT;
  }
}
