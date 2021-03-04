#ifndef WD3_HM_CPP_POINT_H_
#define WD3_HM_CPP_POINT_H_

namespace wd3 {

class point {
public:
  point(const double& depth, const double& value) noexcept;
  point(const point& point) noexcept;
  point& operator=(const point& point) noexcept;
private:
  double _depth;
  double _value;
};

} // namespace wd3

#endif
