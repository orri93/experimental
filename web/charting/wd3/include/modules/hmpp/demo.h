#ifndef WD3_HM_CPP_DEMO_H_
#define WD3_HM_CPP_DEMO_H_

#include <SDL.h>

#include <modules/hmpp/data.h>
#include <modules/hmpp/context.h>

#define WD3_HMPP_DEMO_TYPE_PATTERN  1
#define WD3_HMPP_DEMO_TYPE_ECD      2

namespace wd3 {

class demo {
public:
  demo(::wd3::context& context) noexcept;
  bool create(const int& type, const int& count, const int& step = 60);
  bool loop();

private:
  bool work();
  bool next();
  bool handle(const SDL_Event& event);
  bool onkey(Uint8* state);

  ::wd3::context& _context;

  int _type;
  bool _isgo;
  bool _isrun;

  SDL_Event _event;
};

} // namespace wd3

#endif
