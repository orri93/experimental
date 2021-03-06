#include <cstdlib>
#include <cmath>

#include <iostream>

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
static void range(gos_range_1d& range, const double& from = 0.0, const double& to = 1.0) noexcept;
static void scale(gos_scale& scale) noexcept;
static void screen(gos_screen& screen, const int& width = 0, const int& height = 0) noexcept;
} // namespace initialize

namespace update {
static void scale(gos_scale& scale, const gos_range_1d& domain, const int maximum);
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
  if (_screen.width > 0 && _screen.height > 0) {
    _surface = SDL_SetVideoMode(
      _screen.width,    /* Width */
      _screen.height,   /* Height */
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
  _screen.width = width;
  _screen.height = height;
}

void context::parse(int argc, char** argv) {
  if (argc > 1) {
    const char* a1 = argv[1];
    if (gos_text_are_all_char_digits(a1)) {
      _screen.width = ::atoi(a1);
      std::cout << "Setting screen width to " << _screen.width << std::endl;
    }
  }
  if (argc > 2) {
    const char* a2 = argv[2];
    if (gos_text_are_all_char_digits(a2)) {
      _screen.width = ::atoi(a2);
    }
  }
}

void context::updatexscale(const gos_range_1d& domain) {
  update::scale(_xscale, domain, _screen.width);
}

void context::updateyscale(const gos_range_1d& domain) {
  update::scale(_yscale, domain, _screen.height);
}

void context::updatezscale(const gos_range_1d& domain, const int& count) {
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
    for (int x = 0; x < _screen.width; x++) {
      if (next != end) {
        t = gos_scale_reverse(&_xscale, static_cast<double>(x));
        while (gos_nearest_neighbor(it->time(), next->time(), t)) {
          it = next;
          next = it + 1;
          i++;
          if (next == end) {
            break;
          }
        }
      }
      for (int y = 0; y < _screen.height; y++) {
        fy = static_cast<double>(y);
        fz = determination(*it, fy);
        if (::isfinite(fz)) {
          k = static_cast<int>(gos_scale_value(&_zscale, fz));
          const gos_rgb& rgb = gradient.get().gradient[k];
          pixel = SDL_MapRGB(_surface->format, rgb.r, rgb.g, rgb.b);
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
  double y = gos_scale_reverse(&_yscale, value);
  return column.determination(y);
}

void context::drawpixel(const int& x, const int& y, const Uint32& pixel) {
  *(((Uint32*)(_surface->pixels)) +
    gos_screen_index(&_screen, x, y)) = pixel;
}

namespace initialize {

void range(gos_range_1d& range, const double& from, const double& to) noexcept {
  range.from = from;
  range.to = to;
}

void scale(gos_scale& scale) noexcept {
  range(scale.domain);
  range(scale.range);
}

void screen(gos_screen& screen, const int& width, const int& height) noexcept {
  screen.width = width;
  screen.height = height;
}

} // namespace initialize

namespace update {
void scale(gos_scale& scale, const gos_range_1d& domain, const int maximum) {
  scale.domain.from = domain.from;
  scale.domain.to = domain.to;
  scale.range.from = 0.0;
  scale.range.to = static_cast<double>(maximum) - 1.0;
}
} // namespace update

} // namespace wd3
