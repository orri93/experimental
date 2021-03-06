#ifndef WD3_HM_CPP_GRADIENT_H_
#define WD3_HM_CPP_GRADIENT_H_

#include <cstdint>

#include <vector>

#include <gos/types.h>

namespace wd3 {

typedef ::std::vector<gos_rgb> RgbVector;
typedef ::std::vector<int> IntegerVector;

class gradient {
public:
  gradient() noexcept;
  ~gradient();
  void add(const uint8_t& red, const uint8_t& green, const uint8_t& blue, const int& size = 64);
  void add(const uint32_t& color, const int& size = 64);
  void stock(const int& size = 64);
  bool create();
  const gos_rgb_gradient& get() const;
  void reset();
  void shutdown();
private:
  gos_rgb_gradient _gradient;
  IntegerVector _sizes;
  RgbVector _stops;
};

} // namespace wd3

#endif
