#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include <modules/hm/hmdemo.h>
#endif

#include <modules/hm/hm.h>

// #define WD3_MD_HM_MESSAGE_SIZE 1024

#define GHM_DEFAULT_WIDTH            600
#define GHM_DEFAULT_HEIGHT           400

#define GHM_SUCCESS                    0
#define GHM_FAILURE                   -1

static wd3hm context;

#ifdef __EMSCRIPTEN__

int main(int argc, char** argv) {
  printf("Initialize the EFD WASM Heatmap\n");
  ghm_initialize(&context, GHM_DEFAULT_WIDTH, GHM_DEFAULT_HEIGHT);
  ghm_parse_argument(&context, argc, argv);
  return ghm_create(&context) ? EXIT_SUCCESS : EXIT_FAILURE;
}

EMSCRIPTEN_KEEPALIVE int change(int count, bool copy) {
  if (copy) {
    printf("Change the count from %d to %d with copy\n", context.count, count);
  } else {
    printf("Change the count from %d to %d\n", context.count, count);
  }
  return ghm_change(&context, count, copy) ? GHM_SUCCESS : GHM_FAILURE;
}

EMSCRIPTEN_KEEPALIVE int set(int index, double x, double y) {
  return ghm_data_set(&context, index, x, y) ? GHM_SUCCESS : GHM_FAILURE;
}

EMSCRIPTEN_KEEPALIVE int check() {
  return ghm_data_check(&context) ? GHM_SUCCESS : GHM_FAILURE;
}

EMSCRIPTEN_KEEPALIVE void insert(int at) {
  ghm_buffer_insert(&context, at);
}

EMSCRIPTEN_KEEPALIVE void insertLast() {
  ghm_buffer_insert_last(&context);
}

EMSCRIPTEN_KEEPALIVE void shift() {
  ghm_buffer_shift(&context);
}

EMSCRIPTEN_KEEPALIVE void rescale() {
  ghm_rescale(&context);
}

EMSCRIPTEN_KEEPALIVE void render() {
  ghm_render_all(&context);
}

EMSCRIPTEN_KEEPALIVE void separation() {
  ghm_render_separation(&context);
}

EMSCRIPTEN_KEEPALIVE int setUndefinedColor(const char* color) {
  return ghm_set_undefined(&context, color) ? GHM_SUCCESS : GHM_FAILURE;
}

EMSCRIPTEN_KEEPALIVE int setSeparationColor(int index, const char* color) {
  if (index >= 0) {
    return ghm_set_separation(&context, index, color) ? GHM_SUCCESS : GHM_FAILURE;
  } else {
    return ghm_set_all_separation(&context, color) ? GHM_SUCCESS : GHM_FAILURE;
  }
}

EMSCRIPTEN_KEEPALIVE int setStopCount(int count, bool copy) {
  if (copy) {
    printf(
      "Change the color stop count from %d to %d with copy\n",
      context.stops,
      count);
  } else {
    printf("Change the color stop count from %d to %d\n",
      context.stops,
      count);
  }
  return ghm_change_stops(&context, count, copy) ? GHM_SUCCESS : GHM_FAILURE;
}

EMSCRIPTEN_KEEPALIVE int setStopColor(int index, const char* color) {
  return ghm_set_stop(&context, index, color) ? GHM_SUCCESS : GHM_FAILURE;
}

EMSCRIPTEN_KEEPALIVE void setStopSize(int index, int size) {
  ghm_set_size(&context, index, size);
}

EMSCRIPTEN_KEEPALIVE int createColorGradient() {
  ghm_shutdown_gradient(&context);
  return ghm_create_gradient(&context) ? GHM_SUCCESS : GHM_FAILURE;
}

EMSCRIPTEN_KEEPALIVE int lock() {
  return ghm_lock(&context) ? GHM_SUCCESS : GHM_FAILURE;
}

EMSCRIPTEN_KEEPALIVE int flipUnlock() {
  if (ghm_flip(&context)) {
    ghm_unlock(&context);
    return GHM_SUCCESS;
  } else {
    return GHM_FAILURE;
  }
}

EMSCRIPTEN_KEEPALIVE double getDomain(int flag) {
  return ghm_domain_get(&context, flag);
}

EMSCRIPTEN_KEEPALIVE void setDomain(int flag, double value) {
  ghm_domain_set(&context, flag, value);
}

EMSCRIPTEN_KEEPALIVE int getScreenWidth() {
  return context.screen.width;
}

EMSCRIPTEN_KEEPALIVE int getScreenHeigh() {
  return context.screen.height;
}

EMSCRIPTEN_KEEPALIVE void warning(const char* message) {
  fprintf(stderr, "%s\n", message);
}

EMSCRIPTEN_KEEPALIVE void shutdown() {
  printf("Shutting down the GOS WASM Heatmap\n");
  ghm_shutdown(&context);
}

#else /* __EMSCRIPTEN__ */

static wd3hmdemo demo;

#ifdef WD3_USE_SDL_MAIN
int SDL_main(int argc, char** argv) {
  printf("Initialize the EFD WASM Heatmap Demo with SDL Main\n");
#else
int main(int argc, char** argv) {
  printf("Initialize the EFD WASM Heatmap Demo\n");
#endif
  ghm_initialize(&context, GHM_DEFAULT_WIDTH, GHM_DEFAULT_HEIGHT);
  ghm_parse_argument(&context, argc, argv);

  ghmd_initialize(&demo);

  if (ghm_create(&context)) {
    if (ghmd_create(&context, &demo, WD3_HM_DEMO_TYPE_ECD, 4)) {

      if (!ghmd_render(&context, &demo)) {
        ghmd_shutdown(&demo);
        ghm_shutdown(&context);
        return EXIT_FAILURE;
      }

      if (!ghmd_loop(&context, &demo)) {
        ghmd_shutdown(&demo);
        ghm_shutdown(&context);
        return EXIT_FAILURE;
      }

      ghmd_shutdown(&demo);
    }
    ghm_shutdown(&context);
  } else {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

#endif  /* __EMSCRIPTEN__ */
