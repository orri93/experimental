#ifndef WD3_HM_CPP_CONTEXT_H_
#define WD3_HM_CPP_CONTEXT_H_

#include <SDL.h>

#include <gos/types.h>

#include <modules/hmpp/data.h>

namespace wd3 {

class context {
public:
  context() noexcept;
  ~context();
  bool initialize();
  bool create();
  void set(const int& width, const int& height);
  void parse(int argc, char** argv);
  void shutdown();

private:
  int _sdlinit;
  gos_screen _screen;

  /* Scales */
  gos_scale _xscale;
  gos_scale _yscale;
  gos_scale _zscale;

  /* Colors */
  gos_rgb_gradient _gradient;

  SDL_Surface* _surface;
};

} // namespace wd3

#endif
