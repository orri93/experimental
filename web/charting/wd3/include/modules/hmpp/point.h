#ifndef WD3_HM_CPP_POINT_H_
#define WD3_HM_CPP_POINT_H_

#include <memory>
#include <vector>

namespace wd3 {

class point {
public:
  point(const double& depth, const double& value) noexcept;
  point(const point& point) noexcept;
  point& operator=(const point& point) noexcept;
  void set(const double& depth, const double& value) noexcept;
  const double& depth() const noexcept;
  const double& value() const noexcept;
private:
  double _depth;
  double _value;
};

typedef ::std::unique_ptr<point> PointPtr;
typedef ::std::unique_ptr<::wd3::PointPtr[]> PointsPtr;

typedef ::std::vector<PointPtr> PointPtrVector;

} // namespace wd3

#endif
