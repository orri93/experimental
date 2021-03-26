#ifndef WD3_HM_CPP_CONTEXT_H_
#define WD3_HM_CPP_CONTEXT_H_

#include <chrono>

#include <SDL.h>

#include <gos/time.h>
#include <gos/range.h>
#include <gos/scale.h>
#include <gos/screen.h>

#include <modules/hmpp/types.h>
#include <modules/hmpp/column.h>
#include <modules/hmpp/gradient.h>
#include <modules/hmpp/time.h>
#include <modules/hmpp/data.h>

namespace wd3 {

class context {
public:
  context() noexcept;
  ~context();
  bool initialize();
  bool create();
  void setDefaultColor(const uint32_t& color);
  bool setDefaultColorText(const ::std::string& color);
  void set(const int& width, const int& height);
  void parse(int argc, char** argv);
  void updatexscale(const ::gos::range::d1<::wd3::type::time>& domain);
  void updateyscale(const ::gos::range::d1<double>& domain);
  void updatezscale(const ::gos::range::d1<double>& domain, const int& count);
  const ::wd3::time::scale<int>& getxscale() const;
  const ::gos::scale<double, int>& getyscale() const;
  const ::gos::scale<double, int>& getzscale() const;
  void getzcolor(::wd3::gradient& gradient, ::gos::color::rgb<>& rgb, const double& value);
  bool begin();
  void clear(const int& value = 0);
  bool render(::wd3::gradient& gradient, ::wd3::data& data);
  bool complete();
  void shutdown();

private:
  Uint32 pixel(const ::gos::color::rgb<>& rgb);
  double determination(::wd3::column& column, const int& value);
  void drawpixel(const int& x, const int& y, const Uint32& pixel);
  int _sdlinit;
  ::gos::screen<> _screen;

  SDL_Surface* _surface;

  /* Scales */
  ::wd3::time::scale<int> _xscale;
  ::gos::scale<double, int> _yscale;
  ::gos::scale<double, int> _zscale;

  /* Colors */
  Uint32 _undefined;
};

} // namespace wd3

#endif
