#include <cfloat>

#include <vector>

#include <modules/hmpp/data.h>

namespace wd3 {

static void reset(::gos::range::d1<>& range);
static void update(::gos::range::d1<>& range, const double& value);

data::data(const int& size) noexcept :
  _size(size),
  _time({ DBL_MAX, -DBL_MAX }) {
}

void data::add(const column& column) {
  const double& t = column.time();
  _columns.push_back(column);
  update(_time, t);
}

void data::remove(const double& time) {
  ColumnIterator it = _columns.begin();
  while (it != _columns.end()) {
    if (it->time() <= time) {
      _columns.pop_front();
      it = _columns.begin();
    } else {
      break;
    }
  }
  updatetimerange();
}

void data::ranges(::gos::range::d1<>& depth, ::gos::range::d1<>& value) {
  int i, size;
  reset(depth);
  reset(value);
  for (column& column : _columns) {
    size = column.size();
    for (i = 0; i < size; i++) {
      if (column.is(i)) {
        const point& p = column.at(i);
        update(depth, p.depth());
        update(value, p.value());
      }
    }
  }
}

void data::range(::gos::range::d1<>& depth) {
  int i, size;
  reset(depth);
  for (column& column : _columns) {
    size = column.size();
    for (i = 0; i < size; i++) {
      if (column.is(i)) {
        const point& p = column.at(i);
        update(depth, p.depth());
      }
    }
  }
}

const ::gos::range::d1<>& data::time() const { return _time; }

ColumnIterator data::first() {
  return _columns.begin();
}

ColumnIterator data::last() {
  return _columns.end();
}

void data::updatetimerange() {
  reset(_time);
  for (column& column : _columns) {
    update(_time, column.time());
  }
}

void reset(::gos::range::d1<>& range) {
  range.setfrom(DBL_MAX);
  range.setto(-DBL_MAX);
}

void update(::gos::range::d1<>& range, const double& value) {
  if (value < range.from()) {
    range.setfrom(value);
  }
  if (value > range.to()) {
    range.setto(value);
  }
}

} // namespace wd3
