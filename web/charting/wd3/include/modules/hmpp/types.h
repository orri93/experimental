#ifndef WD3_HM_CPP_DATA_H_
#define WD3_HM_CPP_DATA_H_

#include <vector>

namespace wd3 {
namespace data {

struct Point {
  double depth;
  double value;
};

typedef std::vector<Point> PointVector;

class Group {
private:
  PointVector _points;
  double time;
};

typedef std::vector<Group> GroupVector;

} // namespace data
} // namespace wd3

#endif
