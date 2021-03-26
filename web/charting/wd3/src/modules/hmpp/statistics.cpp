#include <cmath>

#include <modules/hmpp/statistics.h>

namespace wd3 {
namespace statistics {
void sum(wd3::data& data, wd3::column& sum, wd3::column& squaresum) {
  double depth, value;
  int i, size;
  size = data.size();
  wd3::ColumnIterator it = data.first();
  while (it != data.last()) {
    for (i = 0; i < size; i++) {
      if (it->is(i)) {
        wd3::point point = it->at(i);
        depth = point.depth();
        value = point.value();
        if (sum.is(i)) {
          wd3::point sumpoint = sum.at(i);
          wd3::point squaresumpoint = squaresum.at(i);
          sum.set(
            i,
            sumpoint.depth() + depth,
            sumpoint.value() + value);
          squaresum.set(
            i,
            squaresumpoint.depth() + depth * depth,
            squaresumpoint.value() + value * value);
        } else {
          sum.set(
            i,
            depth,
            value);
          squaresum.set(
            i,
            depth * depth,
            value * value);
        }
      }
    }
    it++;
  }
}
void mean(wd3::column& mean, wd3::column& sum, const int& size, const int& count) {
  for (int i = 0; i < size; i++) {
    if (sum.is(i)) {
      wd3::point point = sum.at(i);
      mean.set(i, point.depth() / count, point.value() / count);
    }
  }
}
void diffsquare(wd3::data& data, wd3::column& mean, wd3::column& diffsquare) {
  double depthdiff, valuediff;
  int i, size;
  size = data.size();
  wd3::ColumnIterator it = data.first();
  while (it != data.last()) {
    for (i = 0; i < size; i++) {
      if (it->is(i) && mean.is(i)) {
        wd3::point point = it->at(i);
        wd3::point meanpoint = mean.at(i);
        depthdiff = point.depth() - meanpoint.depth();
        valuediff = point.value() - meanpoint.value();
        if (diffsquare.is(i)) {
          wd3::point point = diffsquare.at(i);
          diffsquare.set(
            i,
            point.depth() + depthdiff * depthdiff,
            point.value() + valuediff * valuediff);
        } else {
          diffsquare.set(
            i,
            depthdiff * depthdiff,
            valuediff * valuediff);
        }
      }
    }
    it++;
  }
}
void sd(::wd3::column& sd, ::wd3::column& diffsquare, const int& size, const int& count) {
  for (int i = 0; i < size; i++) {
    if (diffsquare.is(i)) {
      wd3::point point = diffsquare.at(i);
      sd.set(i, ::sqrt(point.depth() / count), ::sqrt(point.value() / count));
    }
  }
}
} // namespace statistics
} // namespace wd3
