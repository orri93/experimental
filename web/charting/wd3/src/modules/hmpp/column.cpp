#include <cmath>

#include <algorithm>

#include <gos/interpolate.h>

#include <modules/hmpp/exception.h>
#include <modules/hmpp/column.h>

namespace wd3 {

namespace convert {
namespace to {
static void vector(PointPtrVector& vector, PointsPtr& points, const int& count);
} // namespace to
namespace from {
static void vector(PointsPtr& points, PointPtrVector& vector);
} // namespace from
} // namespace convert
namespace compare {
static bool points(const PointPtr& a, const PointPtr& b);
} // namespace compare

column::column(const int& size, const ::wd3::type::time& time) :
  _time(time),
  _size(size) {
  _points = std::make_unique<PointPtr[]>(_size);
  if (!_points) {
    throw ::wd3::exception("Failed to assign memory for points");
  }
}

column::column(const column& column) {
  assign(column);
}

column& column::operator=(const column& column) {
  if (this != &column) {
    assign(column);
  }
  return *this;
}

bool column::operator==(const column& column) const {
  return _time == column._time;
}

bool column::operator!=(const column& column) const {
  return _time != column._time;
}

bool column::operator<(const column& column) const {
  return _time < column._time;
}

bool column::operator>(const column& column) const {
  return _time > column._time;
}

double column::determination(const double& value) {
  int i = index(value);
  return i > 0 ? interpolate(i, value) : NAN;
}

void column::set(const int& index, const double& depth, const double& value) {
  if (_points[index]) {
    _points[index]->set(depth, value);
  } else {
    _points[index] = std::make_unique<point>(depth, value);
  }
}

const bool column::is(const int& index) const {
  return (bool)(_points[index]);
}

const point& column::at(const int& index) const {
  if (_points[index]) {
    return *(_points[index]);
  } else {
    throw ::wd3::exception("Failed to accessing point");
  }
}

const ::wd3::type::time& column::time() const { return _time; }
const int& column::size() const { return _size; }

void column::sort() {
  wd3::PointPtrVector vector;
  wd3::convert::to::vector(vector, _points, _size);
  std::sort(vector.begin(), vector.end(), wd3::compare::points);
  wd3::convert::from::vector(_points, vector);
}

void column::assign(const column& column) {
  _time = column._time;
  _size = column._size;
  _points = std::make_unique<PointPtr[]>(_size);
  if (_points) {
    for (int i = 0; i < _size; i++) {
      if (column.is(i)) {
        const point& p = column.at(i);
        _points[i] = std::make_unique<point>(p);
      }
    }
  } else {
    throw ::wd3::exception("Failed to assign memory for points");
  }
}

int column::index(const double& value) const {
  for (int i = 1; i < _size; i++) {
    if (is(i - 1) && is(i)) {
      if (value >= _points[i - 1]->depth() && value < _points[i]->depth()) {
        return i;
      }
    }
  }
  return -1;
}

double column::interpolate(const int& index, const double& value) {
  const point& first = *(_points[index - 1]);
  const point& second = *(_points[index]);
  return interpolate(first, second, value);
}

double column::interpolate(const point& first, const point& second, const double& value) {
  double mu = (value - first.depth()) / (second.depth() - first.depth());
  return gos::interpolate::linear(first.value(), second.value(), mu);
}

namespace convert {
namespace to {
void vector(PointPtrVector& vector, PointsPtr& points, const int& count) {
  for (int i = 0; i < count; i++) {
    vector.push_back(std::move(points[i]));
  }
}
} // namespace to
namespace from {
void vector(PointsPtr& points, PointPtrVector& vector) {
  int i = 0;
  for (PointPtr& point : vector) {
    points[i++] = std::move(point);
  }
}
} // namespace from
} // namespace convert
namespace compare {
bool points(const PointPtr& a, const PointPtr& b) {
  if (((bool)a) && ((bool)b)) {
    return a->depth() < b->depth();
  } else {
    return (bool)a;
  }
}
} // namespace compare

} // namespace wd3
