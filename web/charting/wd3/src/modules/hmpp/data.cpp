#include <cfloat>

#include <vector>

#include <modules/hmpp/data.h>

namespace wd3 {

static void reset(gos_range_1d& range);
static void update(gos_range_1d& range, const double& value);

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

void data::ranges(gos_range_1d& depth, gos_range_1d& value) {
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

void data::range(gos_range_1d& depth) {
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

const gos_range_1d& data::time() const { return _time; }

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

void reset(gos_range_1d& range) {
  range.from = DBL_MAX;
  range.to = -DBL_MAX;
}

void update(gos_range_1d& range, const double& value) {
  if (value < range.from) {
    range.from = value;
  }
  if (value > range.to) {
    range.to = value;
  }
}

} // namespace wd3
