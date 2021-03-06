#ifndef WD3_HM_CPP_CONTEXT_H_
#define WD3_HM_CPP_CONTEXT_H_

#include <SDL.h>

#include <gos/types.h>

#include <modules/hmpp/data.h>
#include <modules/hmpp/column.h>
#include <modules/hmpp/gradient.h>

namespace wd3 {

class context {
public:
  context() noexcept;
  ~context();
  bool initialize();
  bool create();
  void set(const int& width, const int& height);
  void parse(int argc, char** argv);
  void updatexscale(const gos_range_1d& domain);
  void updateyscale(const gos_range_1d& domain);
  void updatezscale(const gos_range_1d& domain, const int& count);
  bool begin();
  bool render(::wd3::gradient& gradient, ::wd3::data& data);
  bool complete();
  void shutdown();

private:
  double determination(::wd3::column& column, const double& value);
  void drawpixel(const int& x, const int& y, const Uint32& pixel);
  int _sdlinit;
  gos_screen _screen;

  SDL_Surface* _surface;

  /* Scales */
  gos_scale _xscale;
  gos_scale _yscale;
  gos_scale _zscale;

  /* Colors */
  Uint32 _undefined;
};

} // namespace wd3

#endif
