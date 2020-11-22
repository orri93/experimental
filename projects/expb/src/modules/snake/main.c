#include <stdlib.h>

#include <modules/snake/engine.h>

#ifdef __EMSCRIPTEN__
#include <modules/snake.h>
#endif

int main(int argc, char** argv) {
#ifdef __EMSCRIPTEN__
  engine_init(
    SNAKE_WASM_ASSETS_IMG_DIR,
    SNAKE_WASM_ASSETS_FONT_DIR,
    800,
    600);
#else
  const char* imgdir = NULL;
  const char* fontdir = NULL;
  if (argc > 1) {
    imgdir = argv[1];
  }
  if (argc > 2) {
    fontdir = argv[2];
  }
  engine_init(imgdir, fontdir, 800, 600);
#endif
  engine_start();
  return EXIT_SUCCESS;
}
