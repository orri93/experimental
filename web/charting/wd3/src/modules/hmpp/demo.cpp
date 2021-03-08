#include <ctime>

#include <memory>

#include <SDL.h>

#include <gos/noise.h>

#include <modules/hmpp/demo.h>

namespace wd3 {

demo::demo(::wd3::context& context, ::wd3::gradient& gradient, ::wd3::data& data) noexcept :
  _context(context),
  _gradient(gradient),
  _data(data),
  _i(0),
  _size(0),
  _count(0),
  _step(0),
  _isgo(false),
  _isrun(false),
  _event() {
}

bool demo::create(const int& type, const int& size, const int& count, const int& step) {
  ::gos::range::d1<> depthrange, valuerange;
  time_t tt, now;
  double td, n, value;
  int i, j;

  _size = size;
  _count = count;
  _step = step;

  _points = std::make_unique<PointPtr[]>(_size);

  value = 5.0 + ::gos::noise::white(GOS_NOISE_DEFAULT_SEED, 0, 0);
  for (i = 0; i < _size; i++) {
    n = 1.0 + static_cast<double>(i);
    _points[i] = std::make_unique<point>(100.0 * n, value);
    value += ::gos::noise::white(GOS_NOISE_DEFAULT_SEED, 0, i) + 10.0 * n;
  }

  _i = 0;
  now = time(NULL);
  tt = now - static_cast<time_t>(_step) * (static_cast<time_t>(_count) - 1);
  while (tt <= now) {
    td = static_cast<double>(tt);
    column column(_size, td);

    for (j = 0; j < _size; j++) {
      column.set(j, _points[j]->depth(), _points[j]->value());
      evolve(*(_points[j]), _i, j, 0.1, 1.0, true);
    }

    _data.add(column);

    tt += _step;
    _i++;
  }

  _data.ranges(depthrange, valuerange);
  _context.updatexscale(_data.time());
  _context.updateyscale(depthrange);
  _context.updatezscale(valuerange, _gradient.get().size());

  _context.begin();
  _context.render(_gradient, _data);
  _context.complete();

  return true;
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
    Uint8* state = SDL_GetKeyState(NULL);
    if (!onkey(state)) {
      return false;
    }
  }
  while (SDL_PollEvent(&_event)) {
    if (!handle(_event)) {
      return false;
    }
  }
  return true;
}

bool demo::next() {
  ::gos::range::d1<> depthrange, valuerange;
  time_t tt, now;
  double td;
  int i;

  now = time(NULL);
  td = static_cast<double>(now);
  column column(_size, td);
  for (i = 0; i < _size; i++) {
    column.set(i, _points[i]->depth(), _points[i]->value());
    evolve(*(_points[i]), _i, i);
  }

  tt = now - static_cast<time_t>(_step) * (static_cast<time_t>(_count) - 1);
  td = static_cast<double>(tt);
  _data.remove(td);
  _data.add(column);

  _i++;

  _data.ranges(depthrange, valuerange);
  _context.updatexscale(_data.time());
  _context.updateyscale(depthrange);
  _context.updatezscale(valuerange, _gradient.get().size());

  _context.begin();
  _context.render(_gradient, _data);
  _context.complete();

  return true;
}

bool demo::handle(const SDL_Event& event) {
  switch (event.type) {
  case SDL_QUIT:
    _isgo = false;
    break;
  case SDL_KEYDOWN:
    if (!keyboard(event.key)) {
      return false;
    }
    break;
  default:
    /* Do nothing */
    break;
  }
  return true;
}

bool demo::keyboard(const SDL_KeyboardEvent& event) {
  switch (event.keysym.sym) {
  case SDLK_q:
    _isgo = false;
    break;
  case SDLK_s:
    _isrun = _isrun ? false : true;
    break;
  // case SDLK_n:
    // next();
    // break;
  default:
    /* Do nothing */
    break;
  }
  return true;
}

bool demo::onkey(Uint8* state) {
  if (state[SDLK_n]) {
    if (!next()) {
      return false;
    }
  }
  return true;
}

void demo::evolve(
  ::wd3::point& point,
  const int& x,
  const int& y,
  const double& factor,
  const double& depthfactor,
  const bool& isdepthrandom) {
  double depth = point.depth() + (isdepthrandom ?
    depthfactor + ::gos::noise::white(GOS_NOISE_DEFAULT_SEED, x, y) : depthfactor);
  double value = point.value() +
    ::gos::noise::white(GOS_NOISE_DEFAULT_SEED, x, y) * factor;
  point.set(depth, value);
}

} // namespace wd3
