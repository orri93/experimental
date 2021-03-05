#include <modules/hmpp/point.h>

namespace wd3 {

point::point(const double& depth, const double& value) noexcept :
  _depth(depth),
  _value(value) {
}

point::point(const point& point) noexcept :
  _depth(point._depth),
  _value(point._value) {
}

point& point::operator=(const point& point) noexcept {
  if (this != &point) {
    _depth = point._depth;
    _value = point._value;
  }
  return *this;
}

void point::set(const double& depth, const double& value) noexcept {
  _depth = depth;
  _value = value;
}

const double& point::depth() const noexcept { return _depth; }
const double& point::value() const noexcept { return _value; }

} // namespace wd3
