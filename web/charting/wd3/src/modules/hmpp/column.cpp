#include <modules/hmpp/exception.h>
#include <modules/hmpp/column.h>

namespace wd3 {

column::column(const int& size, const double& time) :
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

const double& column::time() const { return _time; }
const int& column::size() const { return _size; }

} // namespace wd3
