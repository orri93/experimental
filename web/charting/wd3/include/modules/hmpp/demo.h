#ifndef WD3_HM_CPP_DEMO_H_
#define WD3_HM_CPP_DEMO_H_

#include <SDL.h>

#include <modules/hmpp/data.h>
#include <modules/hmpp/context.h>
#include <modules/hmpp/gradient.h>


#define WD3_HMPP_DEMO_TYPE_PATTERN  1
#define WD3_HMPP_DEMO_TYPE_ECD      2

namespace wd3 {

class demo {
public:
  demo(::wd3::context& context, ::wd3::gradient& gradient, ::wd3::data& data) noexcept;
  bool create(const int& type, const int& size, const int& count, const int& step = 60);
  bool loop();

private:
  bool work();
  bool next();
  bool handle(const SDL_Event& event);
  bool keyboard(const SDL_KeyboardEvent& event);
  bool onkey(Uint8* state);
  static void evolve(
    ::wd3::point& point,
    const int& x,
    const int& y,
    const double& factor = 0.1,
    const double& depthfactor = 0.1,
    const bool& isdepthrandom = false);

  ::wd3::context& _context;
  ::wd3::gradient& _gradient;
  ::wd3::data& _data;

  ::wd3::PointsPtr _points;

  int _i;
  int _size;
  int _count;
  int _step;
  bool _isgo;
  bool _isrun;

  SDL_Event _event;
};

} // namespace wd3

#endif
