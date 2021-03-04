#include <cstdlib>

#include <iostream>

#include <SDL.h>

#include <gos/text.h>

#include <modules/hmpp/context.h>

#define WD3_SDL_SUCCESS               0
#define WD3_SDL_INIT_DEFAULT         -1
#define WD3_SDL_BPP                  32

namespace wd3 {

namespace initialize {
static void range(gos_range_1d& range, const double& from = 0.0, const double& to = 1.0) noexcept;
static void scale(gos_scale& scale) noexcept;
static void screen(gos_screen& screen, const int& width = 0, const int& height = 0) noexcept;
static void gradient(gos_rgb_gradient& gradient) noexcept;
} // namespace initialize

context::context() noexcept :
  _sdlinit(WD3_SDL_INIT_DEFAULT),
  _surface(NULL) {
  ::wd3::initialize::screen(_screen);
  ::wd3::initialize::scale(_xscale);
  ::wd3::initialize::scale(_yscale);
  ::wd3::initialize::scale(_zscale);
  ::wd3::initialize::gradient(_gradient);
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


void context::shutdown() {
  if (_sdlinit == WD3_SDL_SUCCESS) {
    SDL_Quit();
    _sdlinit = WD3_SDL_INIT_DEFAULT;
    _surface = NULL;
  }
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

void gradient(gos_rgb_gradient& gradient) noexcept {
  gradient.gradient = NULL;
  gradient.count = 0;
}

} // namespace initialize

} // namespace wd3
