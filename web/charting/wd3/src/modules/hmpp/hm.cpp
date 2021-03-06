#include <cstdlib>

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#ifdef WD3_USE_SDL_MAIN
#include <SDL_main.h>
#endif
#include <modules/hmpp/demo.h>
#endif

#include <modules/macros.h>
#include <modules/hmpp/data.h>
#include <modules/hmpp/context.h>
#include <modules/hmpp/gradient.h>

const int DataSize = 4;
const int DataCount = 100;

static wd3::context context;
static wd3::gradient gradient;
static wd3::data data(DataSize);

#ifdef __EMSCRIPTEN__

int main(int argc, char** argv) {
  std::cout << "Initialize the WD3 WASM Heatmap" << std::endl;
  return EXIT_SUCCESS;
}

#else

static wd3::demo demo(context, gradient, data);

#ifdef WD3_USE_SDL_MAIN
int SDL_main(int argc, char** argv) {
  std::cout << "Initialize the WD3 WASM Heatmap Demo with SDL Main" << std::endl;
#else
int main(int argc, char** argv) {
  std::cout << "Initialize the WD3 WASM Heatmap Demo" << std::endl;
#endif
  context.set(WD3_DEFAULT_WIDTH, WD3_DEFAULT_HEIGHT);
  context.parse(argc, argv);
  gradient.stock();
  if (gradient.create()) {
    if (context.initialize()) {
      if (context.create()) {
        if (demo.create(WD3_HMPP_DEMO_TYPE_PATTERN, DataSize, DataCount)) {
          if (demo.loop()) {
            return EXIT_SUCCESS;
          }
        }
      }
    }
  }
  return EXIT_FAILURE;
}

#endif
