#include <cmath>

#include <gos/interpolate.h>

#include <modules/hmpp/exception.h>
#include <modules/hmpp/column.h>

namespace wd3 {

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
    if (value >= _points[i - 1]->depth() && value < _points[i]->depth()) {
      return i;
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

const ::wd3::type::time& column::time() const { return _time; }
const int& column::size() const { return _size; }

} // namespace wd3
