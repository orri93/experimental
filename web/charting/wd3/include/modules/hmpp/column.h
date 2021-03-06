#ifndef WD3_HM_CPP_COLUMN_H_
#define WD3_HM_CPP_COLUMN_H_

#include <cstddef>

#include <deque>
#include <memory>

#include <modules/hmpp/point.h>

namespace wd3 {

class column {
public:
  column(const int& size, const double& time);
  column(const column& column);
  column& operator=(const column& column);
  double determination(const double& value);
  void set(const int& index, const double& depth, const double& value);
  const bool is(const int& index) const;
  const point& at(const int& index) const;
  const double& time() const;
  const int& size() const;

private:
  void assign(const column& column);
  int index(const double& value) const;
  double interpolate(const int& index, const double& value);
  static double interpolate(const point& first, const point& second, const double& value);
  double _time;
  int _size;
  PointsPtr _points;
};

typedef ::std::deque<column> ColumnQueue;
typedef ColumnQueue::iterator ColumnIterator;

} // namespace wd3

#endif
