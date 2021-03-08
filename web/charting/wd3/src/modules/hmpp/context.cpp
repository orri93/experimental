#include <cstdlib>
#include <cmath>

#include <iostream>
#include <string>

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

namespace initialize {
static void range(::gos::range::d1<>& range, const double& from = 0.0, const double& to = 1.0) noexcept;
static void scale(::gos::scale<>& scale) noexcept;
static void screen(::gos::screen<>& screen, const int& width = 0, const int& height = 0) noexcept;
} // namespace initialize

namespace update {
static void scale(::gos::scale<>& scale, const ::gos::range::d1<>& domain, const int maximum);
} // namespace update

context::context() noexcept :
  _sdlinit(WD3_SDL_INIT_DEFAULT),
  _surface(NULL),
  _undefined(0) {
  ::wd3::initialize::screen(_screen);
  ::wd3::initialize::scale(_xscale);
  ::wd3::initialize::scale(_yscale);
  ::wd3::initialize::scale(_zscale);
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

void context::updatexscale(const ::gos::range::d1<>& domain) {
  update::scale(_xscale, domain, _screen.width());
}

void context::updateyscale(const ::gos::range::d1<>& domain) {
  update::scale(_yscale, domain, _screen.height());
}

void context::updatezscale(const ::gos::range::d1<>& domain, const int& count) {
  update::scale(_zscale, domain, count);
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
  int k, i = 0;
  Uint32 pixel;
  double t, fz, fy;
  ColumnIterator it = data.first();
  ColumnIterator end = data.last();
  if (it != end) {
    ColumnIterator next = it + 1;
    for (int x = 0; x < _screen.width(); x++) {
      if (next != end) {
        t = _xscale.reverse(static_cast<double>(x));
        while (::gos::nearest::neighbor(it->time(), next->time(), t)) {
          it = next;
          next = it + 1;
          i++;
          if (next == end) {
            break;
          }
        }
      }
      for (int y = 0; y < _screen.height(); y++) {
        fy = static_cast<double>(y);
        fz = determination(*it, fy);
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

double context::determination(column& column, const double& value) {
  double y = _yscale.reverse(value);
  return column.determination(y);
}

void context::drawpixel(const int& x, const int& y, const Uint32& pixel) {
  *(((Uint32*)(_surface->pixels)) + _screen.index(x, y)) = pixel;
}

namespace initialize {
void range(::gos::range::d1<>& range, const double& from, const double& to) noexcept {
  range.setfrom(from);
  range.setto(to);
}
void scale(::gos::scale<>& scale) noexcept {
  ::gos::range::d1<> domain, range;
  ::wd3::initialize::range(domain);
  ::wd3::initialize::range(range);
  scale.setdomain(domain);
  scale.setrange(range);
}
void screen(::gos::screen<>& screen, const int& width, const int& height) noexcept {
  screen.setwidth(width);
  screen.setheight(height);
}
} // namespace initialize

namespace update {
void scale(::gos::scale<>& scale, const ::gos::range::d1<>& domain, const int maximum) {
  scale.setdomain(domain);
  scale.setrange(0.0, static_cast<double>(maximum) - 1.0);
}
} // namespace update

} // namespace wd3
