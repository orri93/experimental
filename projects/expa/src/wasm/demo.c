#include <wasm/demo.h>
#include <wasm/heatmap.h>

bool gos_heatmap_demo(gos_expa_data* expad, int width, int height) {
  bool move, go;
  SDL_Event event;

  if (gos_heatmap_initialize(width, height)) {
    gos_heatmap_draw();

    move = false;
    go = true;
    while (go) {
      if (move) {
        gos_heatmap_line();
      }

      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
          case SDLK_s:
            move = move ? false : true;
            break;
          case SDLK_q:
            go = false;
            break;
          default:
            break;
          }
          break;
        case SDL_QUIT:
          go = false;
          break;
        default:
          break;
        }
      }
    }

    //gos_heatmap_step();
    //gos_heatmap_step();
    //gos_heatmap_step();
    //gos_heatmap_step();

    gos_heatmap_shutdown();

    return true;
  }

  return false;
}
