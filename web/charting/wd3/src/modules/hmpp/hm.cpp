#include <cstdlib>

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <modules/hmpp/emscripten.h>
#else
#ifdef WD3_USE_SDL_MAIN
#include <SDL_main.h>
#endif
#include <modules/hmpp/demo.h>
#endif

#ifdef __EMSCRIPTEN__
int main(int argc, char** argv) {
  std::cout << "Initialize the WD3 WASM Heatmap" << std::endl;
  return ::wd3::emscripten::main(argc, argv);
}
#else
#ifdef WD3_USE_SDL_MAIN
int SDL_main(int argc, char** argv) {
  std::cout << "Initialize the WD3 WASM Heatmap Demo with SDL Main" << std::endl;
#else
int main(int argc, char** argv) {
  std::cout << "Initialize the WD3 WASM Heatmap Demo" << std::endl;
#endif
  return wd3::demo::dmain(argc, argv);
}
#endif
