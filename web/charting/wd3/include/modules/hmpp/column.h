#ifndef WD3_HM_CPP_COLUMN_H_
#define WD3_HM_CPP_COLUMN_H_

#include <cstddef>
#include <ctime>

#include <deque>
#include <memory>

#include <modules/hmpp/types.h>
#include <modules/hmpp/point.h>

namespace wd3 {

class column {
public:
  column(const int& size, const ::wd3::type::time& time);
  column(const column& column);
  column& operator=(const column& column);
  bool operator==(const column& column) const;
  bool operator!=(const column& column) const;
  bool operator<(const column& column) const;
  bool operator>(const column& column) const;
  double determination(const double& value);
  void set(const int& index, const double& depth, const double& value);
  const bool is(const int& index) const;
  const point& at(const int& index) const;
  const ::wd3::type::time& time() const;
  const int& size() const;
  void sort();

private:
  void assign(const column& column);
  int index(const double& value) const;
  double interpolate(const int& index, const double& value);
  static double interpolate(const point& first, const point& second, const double& value);
  ::wd3::type::time _time;
  int _size;
  PointsPtr _points;
};

typedef ::std::deque<column> ColumnQueue;
typedef ColumnQueue::iterator ColumnIterator;
typedef ColumnQueue::size_type ColumnSize;

} // namespace wd3

#endif
