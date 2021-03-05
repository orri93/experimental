#ifndef WD3_HM_CPP_DATA_H_
#define WD3_HM_CPP_DATA_H_

#include <vector>

#include <gos/types.h>

#include <modules/hmpp/column.h>

namespace wd3 {

class data {
public:
  data(const int& size) noexcept;
  void add(const ::wd3::column& column);
  void remove(const double& time);
  void ranges(gos_range_1d& depth, gos_range_1d& value);
  void range(gos_range_1d& depth);

private:
  void updatetimerange();
  int _size;
  ::wd3::ColumnQueue _columns;
  gos_range_1d _time;
};

} // namespace wd3

#endif
