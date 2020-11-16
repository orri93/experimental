#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include <modules/mset.h>
#include <modules/mset/items.h>
#include <modules/mset/loop.h>

static gos_mset_context context;

static void shutdown();

int main(int argc, char** argv) {

  gmset_items_initalize(&context);
  if (!gmset_items_create(&context)) {
    shutdown();
    return EXIT_FAILURE;
  }

  gmset_loop(&context);

  shutdown();
  return EXIT_SUCCESS;
}

void shutdown() {
  gmset_items_shutdown(&context);
}
