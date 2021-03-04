#include <cstdlib>

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else

#endif

#include <modules/macros.h>
#include <modules/hmpp/context.h>

#define GHM_DEFAULT_WIDTH            600
#define GHM_DEFAULT_HEIGHT           400

#define GHM_SUCCESS                    0
#define GHM_FAILURE                   -1

#ifdef __EMSCRIPTEN__

int main(int argc, char** argv) {
  std::cout << "Initialize the EFD WASM Heatmap" << std::endl();
  return EXIT_SUCCESS;
}

#else

#ifdef WD3_USE_SDL_MAIN
int SDL_main(int argc, char** argv) {
  std::cout << "Initialize the EFD WASM Heatmap Demo with SDL Main" << std::endl();
#else
int main(int argc, char** argv) {
  std::cout << "Initialize the EFD WASM Heatmap Demo" << std::endl();
#endif
  std::cout << "Initialize the EFD WASM Heatmap" << std::endl();
  return EXIT_SUCCESS;
}

#endif
