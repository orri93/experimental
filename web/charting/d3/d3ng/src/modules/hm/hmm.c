#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <modules/hm.h>

#define GOS_D3NG_MD_HM_MESSAGE_SIZE 1024

#define GHM_DEMO_X      200
#define GHM_DEMO_Y      200

#define GHM_DEMO_WIDTH  600
#define GHM_DEMO_HEIGHT 400

#define GHM_DEMO_TITLE    "GOS Heatmap"

static goshm context;

#ifdef __EMSCRIPTEN__

int main(int argc, char** argv) {
  printf("Initialize the GOS WASM Heatmap\n");
  ghm_initialize(&context, GHM_DEMO_WIDTH, GHM_DEMO_HEIGHT);
  ghm_parse_argument(&context, argc, argv);

  if (ghm_create(&context, GHM_DEMO_X, GHM_DEMO_Y, GHM_DEMO_TITLE)) {
    if (ghm_create_texture(&context)) {
      if (!ghm_draw(&context)) {
        ghm_shutdown(&context);
        return EXIT_FAILURE;
      }
    } else {
      ghm_shutdown(&context);
      return EXIT_FAILURE;
    }
  } else {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

EMSCRIPTEN_KEEPALIVE void shift() {
  printf("Shift the GOS WASM Heatmap\n");
  if (ghm_shift(&context)) {
    ghm_draw(&context);
  }
}

EMSCRIPTEN_KEEPALIVE void shutdown() {
  printf("Shutting down the GOS WASM Heatmap\n");
  ghm_shutdown(&context);
}

#else /* __EMSCRIPTEN__ */

int main(int argc, char** argv) {
  ghm_initialize(&context, GHM_DEMO_WIDTH, GHM_DEMO_HEIGHT);
  ghm_parse_argument(&context, argc, argv);

  if (ghm_create(&context, GHM_DEMO_X, GHM_DEMO_Y, GHM_DEMO_TITLE)) {
    if (ghm_create_texture(&context)) {
      if (!ghm_draw(&context)) {
        ghm_shutdown(&context);
        return EXIT_FAILURE;
      }
    } else {
      ghm_shutdown(&context);
      return EXIT_FAILURE;
    }

    if (!ghm_loop(&context)) {
      ghm_shutdown(&context);
      return EXIT_FAILURE;
    }
    ghm_shutdown(&context);
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
}

#endif  /* __EMSCRIPTEN__ */

