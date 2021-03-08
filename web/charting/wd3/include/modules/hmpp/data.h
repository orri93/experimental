#ifndef WD3_HM_CPP_DATA_H_
#define WD3_HM_CPP_DATA_H_

#include <vector>

#include <gos/range.h>

#include <modules/hmpp/column.h>

namespace wd3 {

class data {
public:
  data(const int& size) noexcept;
  void add(const ::wd3::column& column);
  void remove(const double& time);
  void ranges(::gos::range::d1<>& depth, ::gos::range::d1<>& value);
  void range(::gos::range::d1<>& depth);
  const ::gos::range::d1<>& time() const;
  ::wd3::ColumnIterator first();
  ::wd3::ColumnIterator last();
  const column& nearest(const double& time);

private:
  void updatetimerange();
  int _size;
  ::wd3::ColumnQueue _columns;
  ::gos::range::d1<> _time;
};

} // namespace wd3

#endif
