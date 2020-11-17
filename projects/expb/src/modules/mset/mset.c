#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <gos/text.h>

#include <modules/mset.h>
#include <modules/mset/items.h>
#include <modules/mset/loop.h>

static gos_mset_context context;

#ifdef __EMSCRIPTEN__

int main(int argc, char** argv) {
  printf("Initialize the Mandelbrot set!\n");
  gmset_items_initalize(&context);

  if (argc > 1) {
    if (gos_text_are_all_char_digits(argv[1])) {
      context.w = atoi(argv[1]);
      printf("Setting width to %d\n", context.w);
    }
  }
  if (argc > 2) {
    if (gos_text_are_all_char_digits(argv[2])) {
      context.h = atoi(argv[2]);
      printf("Setting height to %d\n", context.h);
    }
  }

  if (!gmset_items_create(&context)) {
    return EXIT_FAILURE;
  }

  emscripten_set_main_loop_arg(gmset_loop_wasm, &context, -1, 1);

  return EXIT_SUCCESS;
}

EMSCRIPTEN_KEEPALIVE void shutdown() {
  printf("Shutting down the Mandelbrot set!\n");
  gmset_items_shutdown(&context);
}

#else /* __EMSCRIPTEN__ */

int main(int argc, char** argv) {
  gmset_items_initalize(&context);
  if (!gmset_items_create(&context)) {
    gmset_items_shutdown(&context);
    return EXIT_FAILURE;
  }

  gmset_loop(&context);

  gmset_items_shutdown(&context);
  return EXIT_SUCCESS;
}

#endif  /* __EMSCRIPTEN__ */
