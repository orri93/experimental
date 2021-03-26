#ifndef WD3_HM_CPP_DATA_H_
#define WD3_HM_CPP_DATA_H_

#include <ctime>

#include <gos/range.h>

#include <modules/hmpp/types.h>
#include <modules/hmpp/column.h>

namespace wd3 {

class data {
public:
  data(const int& size) noexcept;
  const int& size() const;
  const ::wd3::ColumnSize count() const;
  void add(const ::wd3::column& column);
  void remove(const ::wd3::type::time& time);
  void ranges(::gos::range::d1<>& depth, ::gos::range::d1<>& value);
  void range(::gos::range::d1<>& depth);
  const ::gos::range::d1<::wd3::type::time>& time() const;
  ::wd3::ColumnIterator first();
  ::wd3::ColumnIterator last();
  void sort();

private:
  void updatetimerange();
  int _size;
  ::wd3::ColumnQueue _columns;
  ::gos::range::d1<::wd3::type::time> _time;
};

} // namespace wd3

#endif
