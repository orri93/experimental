#include <cstdlib>
#include <cmath>

#include <iostream>
#include <string>
#include <chrono>

#include <SDL.h>

#include <gos/types.h>
#include <gos/text.h>
#include <gos/scale.h>
#include <gos/screen.h>
#include <gos/nearest.h>

#include <modules/hmpp/context.h>

#define WD3_SDL_SUCCESS               0
#define WD3_SDL_INIT_DEFAULT         -1
#define WD3_SDL_BPP                  32

namespace wd3 {

namespace nearest {
static ::wd3::type::duration neighbor(
  const ::wd3::type::time& first,
  const ::wd3::type::time& second,
  const ::wd3::type::time& value);
} // namespace nearest

context::context() noexcept :
  _sdlinit(WD3_SDL_INIT_DEFAULT),
  _surface(NULL),
  _undefined(0) {
}

context::~context() {
  shutdown();
}

bool context::initialize() {
  _sdlinit = SDL_Init(SDL_INIT_VIDEO);
  if (_sdlinit == WD3_SDL_SUCCESS) {
    return true;
  } else {
    std::cerr << "SDL initialize failed: " << SDL_GetError() << std::endl;
    return false;
  }
}

bool context::create() {
  if (_screen.width() > 0 && _screen.height() > 0) {
    _surface = SDL_SetVideoMode(
      _screen.width(),  /* Width */
      _screen.height(), /* Height */
      WD3_SDL_BPP,      /* Bits per pixel */
      SDL_SWSURFACE);   /* Flags */
    if (_surface != NULL) {
      return true;
    } else {
      std::cerr << "SDL Set Video Mode failed:" << SDL_GetError() << std::endl;
      return false;
    }
  }
  return true;
}

void context::set(const int& width, const int& height) {
  _screen.setwidth(width);
  _screen.setheight(height);
}

void context::parse(int argc, char** argv) {
  if (argc > 1) {
    std::string a1(argv[1]);
    if (gos::text::are::all::digits(a1)) {
      _screen.setwidth(::atoi(a1.c_str()));
      std::cout << "Setting screen width to " << _screen.width() << std::endl;
    }
  }
  if (argc > 2) {
    std::string a2(argv[1]);
    if (gos::text::are::all::digits(a2)) {
      _screen.setheight(::atoi(a2.c_str()));
      std::cout << "Setting screen height to " << _screen.height() << std::endl;
    }
  }
}

void context::updatexscale(const ::gos::range::d1<::wd3::type::time>& domain) {
  _xscale.setdomain(domain);
  _xscale.setrange(0, _screen.width());
}

void context::updateyscale(const ::gos::range::d1<double>& domain) {
  _yscale.setdomain(domain);
  _yscale.setrange(0, _screen.height());
}

void context::updatezscale(const ::gos::range::d1<double>& domain, const int& count) {
  _zscale.setdomain(domain);
  _zscale.setrange(0, count);
}

bool context::begin() {
  if (SDL_MUSTLOCK(_surface)) {
    if (SDL_LockSurface(_surface) != WD3_SDL_SUCCESS) {
      std::cerr << "SDL Lock Surface failed: " << SDL_GetError() << std::endl;
      return false;
    }
  }
  return true;
}

bool context::render(wd3::gradient& gradient, wd3::data& data) {
  const ::wd3::type::duration moment = ::wd3::type::duration();
  int k, i = 0;
  Uint32 pixel;
  ::wd3::type::time t;
  double fz;
  ColumnIterator it = data.first();
  ColumnIterator end = data.last();
  if (it != end) {
    ColumnIterator next = it + 1;
    for (int x = 0; x < _screen.width(); x++) {
      if (next != end) {
        t = _xscale.reverse(x);
        while (::wd3::nearest::neighbor(it->time(), next->time(), t) > moment) {
          it = next;
          next = it + 1;
          i++;
          if (next == end) {
            break;
          }
        }
      }
      for (int y = 0; y < _screen.height(); y++) {
        fz = determination(*it, y);
        if (::isfinite(fz)) {
          k = static_cast<int>(_zscale.value(fz));
          const ::gos::color::rgb<>& rgb = gradient.get().at(k);
          pixel = SDL_MapRGB(_surface->format, rgb.r(), rgb.g(), rgb.b());
        } else {
          pixel = _undefined;
        }
        drawpixel(x, y, pixel);
      }
    }
  }
  return true;
}

bool context::complete() {
  if (SDL_MUSTLOCK(_surface)) {
    SDL_UnlockSurface(_surface);
  }
  if (SDL_Flip(_surface) != WD3_SDL_SUCCESS) {
    std::cerr << "SDL Flip failed: " << SDL_GetError() << std::endl;
    return false;
  }
  return true;
}

void context::shutdown() {
  if (_sdlinit == WD3_SDL_SUCCESS) {
    SDL_Quit();
    _sdlinit = WD3_SDL_INIT_DEFAULT;
    _surface = NULL;
  }
}

double context::determination(column& column, const int& value) {
  double y = _yscale.reverse(value);
  return column.determination(y);
}

void context::drawpixel(const int& x, const int& y, const Uint32& pixel) {
  *(((Uint32*)(_surface->pixels)) + _screen.index(x, y)) = pixel;
}

namespace nearest {
::wd3::type::duration neighbor(
  const ::wd3::type::time& first,
  const ::wd3::type::time& second,
  const ::wd3::type::time& value) {
  ::wd3::type::duration fd = ::std::chrono::abs(first - value);
  ::wd3::type::duration sd = ::std::chrono::abs(second - value);
  return fd - sd;
}
} // namespace nearest

} // namespace wd3
