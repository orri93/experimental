#include <memory>
#include <algorithm>

#include <modules/hmpp/gradient.h>

#include <gos/color.h>

namespace wd3 {

namespace initialize {
static void gradient(gos_rgb_gradient& gradient) noexcept;
} // namespace initialize

gradient::gradient() noexcept {
  initialize::gradient(_gradient);
}
gradient::~gradient() {
  shutdown();
}

void gradient::add(const uint8_t& red, const uint8_t& green, const uint8_t& blue, const int& size) {
  _stops.push_back({ red, green, blue });
  _sizes.push_back(size);
}

void gradient::add(const uint32_t& color, const int& size) {
  gos_rgb rgb;
  gos_color_assign_rgb32(&rgb, color);
  _stops.push_back(rgb);
  _sizes.push_back(size);
}

void gradient::stock(const int& size) {
  add(0x07049b, size);  /* Dark blue */
  add(0x02f7f3, size);  /* Cyan */
  add(0x09f725, size);  /* Green */
  add(0xf4ec04, size);  /* Yellow */
  add(0xf79d01, size);  /* Orange */
  add(0x8c0101, size);  /* Dark red */
}

bool gradient::create() {
  if (_stops.size() > 0) {
    int count = std::min(static_cast<int>(_stops.size()), static_cast<int>(_sizes.size()));
    std::unique_ptr<gos_rgb[]> stops = std::make_unique<gos_rgb[]>(count);
    std::unique_ptr<int[]> sizes = std::make_unique<int[]>(count);
    if (stops && sizes) {
      for (int i = 0; i < count; i++) {
        stops[i].r = _stops[i].r;
        stops[i].g = _stops[i].g;
        stops[i].b = _stops[i].b;
        sizes[i] = _sizes[i];
      }
      return gos_color_create_rgb_gradient(
        &_gradient,
        stops.get(),
        sizes.get(),
        count,
        GOS_COLOR_GAMMA);
    }
  }
  return false;
}

const gos_rgb_gradient& gradient::get() const { return _gradient; }

void gradient::reset() {
  shutdown();
  _stops.clear();
  _sizes.clear();
}

void gradient::shutdown() {
  gos_color_free_rgb_gradient(&_gradient);
}

namespace initialize {
void gradient(gos_rgb_gradient& gradient) noexcept {
  gradient.gradient = NULL;
  gradient.count = 0;
}
} // namespace initialize

} // namespace wd3
