#include <string>
#include <memory>
#include <algorithm>

#include <modules/hmpp/gradient.h>

#include <gos/color.h>

namespace wd3 {

gradient::gradient() noexcept {
}

void gradient::add(const uint8_t& red, const uint8_t& green, const uint8_t& blue, const int& size) {
  _stops.push_back({ red, green, blue });
  _sizes.push_back(size);
}

void gradient::add(const uint32_t& color, const int& size) {
  _stops.push_back(::gos::color::make_rgb(color));
  _sizes.push_back(size);
}

bool gradient::add(std::string text, const int& size) {
  gos::color::rgb rgb;
  if (rgb.assign(text)) {
    _stops.push_back(rgb);
    _sizes.push_back(size);
    return true;
  } else {
    return false;
  }
}

void gradient::stock(const int& size) {
  add(0x07049b, size);  /* Dark blue */
  add(0x02f7f3, size);  /* Cyan */
  add(0x09f725, size);  /* Green */
  add(0xf4ec04, size);  /* Yellow */
  add(0xf79d01, size);  /* Orange */
  add(0x8c0101, size);  /* Dark red */
}

void gradient::create() {
  ::gos::color::gradient::create(_gradients, _stops, _sizes, GOS_COLOR_GAMMA);
}

const RgbVector& gradient::get() const { return _gradients; }

void gradient::reset() {
  _gradients.clear();
  _stops.clear();
  _sizes.clear();
}

} // namespace wd3
