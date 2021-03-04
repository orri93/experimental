#include <ctime>

#include <SDL.h>

#include <modules/hmpp/demo.h>

namespace wd3 {

demo::demo(::wd3::context& context) noexcept :
  _context(context),
  _type(0),
  _isgo(false),
  _isrun(false),
  _event() {
}

bool demo::create(const int& type, const int& count, const int& step) {
  time_t t, now;

  now = time(NULL);
  t = now - static_cast<time_t>(step) * static_cast<time_t>(count);
  while (t <= now) {

    t += step;
  }
}

bool demo::loop() {
  _isgo = true;
  while (_isgo) {
    if (!work()) {
      return false;
    }
  }
  return true;
}

bool demo::work() {
  if (_isrun) {
    if (!next()) {
      return false;
    }
  } else {

  }
  while (SDL_PollEvent(&_event)) {
    if (!handle(_event)) {
      return false;
    }
  }
  return true;
}

bool demo::next() {

}

bool demo::onkey(Uint8* state) {
  if (state[SDLK_n]) {
    next();
  }
}

} // namespace wd3
