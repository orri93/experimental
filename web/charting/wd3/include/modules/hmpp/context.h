#ifndef WD3_HM_CPP_CONTEXT_H_
#define WD3_HM_CPP_CONTEXT_H_

#include <SDL.h>

#include <gos/range.h>
#include <gos/scale.h>
#include <gos/screen.h>

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
  void updatexscale(const ::gos::range::d1<>& domain);
  void updateyscale(const ::gos::range::d1<>& domain);
  void updatezscale(const ::gos::range::d1<>& domain, const int& count);
  bool begin();
  bool render(::wd3::gradient& gradient, ::wd3::data& data);
  bool complete();
  void shutdown();

private:
  double determination(::wd3::column& column, const double& value);
  void drawpixel(const int& x, const int& y, const Uint32& pixel);
  int _sdlinit;
  ::gos::screen<> _screen;

  SDL_Surface* _surface;

  /* Scales */
  ::gos::scale<> _xscale;
  ::gos::scale<> _yscale;
  ::gos::scale<> _zscale;

  /* Colors */
  Uint32 _undefined;
};

} // namespace wd3

#endif
