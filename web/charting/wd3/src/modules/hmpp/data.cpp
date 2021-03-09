#include <cfloat>

#include <vector>
#include <limits>

#include <modules/hmpp/data.h>

namespace wd3 {

static void reset(::gos::range::d1<>& range);

data::data(const int& size) noexcept :
  _size(size) {
  _time.reset(::wd3::type::time::min(), ::wd3::type::time::max());
}

const int& data::size() const { return _size; }

void data::add(const column& column) {
  _columns.push_back(column);
  _time.update(column.time());
}

void data::remove(const ::wd3::type::time& time) {
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
  depth.reset(-DBL_MAX, DBL_MAX);
  value.reset(-DBL_MAX, DBL_MAX);
  for (column& column : _columns) {
    size = column.size();
    for (i = 0; i < size; i++) {
      if (column.is(i)) {
        const point& p = column.at(i);
        depth.update(p.depth());
        value.update(p.value());
      }
    }
  }
}

void data::range(::gos::range::d1<>& depth) {
  int i, size;
  depth.reset(-DBL_MAX, DBL_MAX);
  for (column& column : _columns) {
    size = column.size();
    for (i = 0; i < size; i++) {
      if (column.is(i)) {
        const point& p = column.at(i);
        depth.update(p.depth());
      }
    }
  }
}

const ::gos::range::d1<::wd3::type::time>& data::time() const { return _time; }

ColumnIterator data::first() {
  return _columns.begin();
}

ColumnIterator data::last() {
  return _columns.end();
}

void data::updatetimerange() {
  _time.reset(::wd3::type::time::min(), ::wd3::type::time::max());
  for (column& column : _columns) {
    _time.update(column.time());
  }
}

void reset(::gos::range::d1<>& range) {
  range.setfrom(DBL_MAX);
  range.setto(-DBL_MAX);
}

} // namespace wd3
