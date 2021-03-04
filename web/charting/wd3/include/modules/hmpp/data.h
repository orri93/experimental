#ifndef WD3_HM_CPP_DATA_H_
#define WD3_HM_CPP_DATA_H_

#include <modules/hmpp/point.h>

namespace wd3 {

class data {
public:
  data(const double& time) noexcept;
  data(const data& data) noexcept;
  data& operator=(const data& data) noexcept;
  void set(const double& time);

private:
  double _time;
};

} // namespace wd3

#endif
