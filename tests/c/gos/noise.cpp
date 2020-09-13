#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/noise.h>

namespace gos {
namespace color {

TEST(GosNoiseTest, White) {
  const int seed = GOS_NOISE_DEFAULT_SEED;
  double n;

  for (int x = 0; x < 32; x++) {
    for (int y = 0; y < 32; y++) {
      n = gos_noise_white(seed, x, y);
      EXPECT_LE(-1.0, n);
      EXPECT_GE(1.0, n);
    }
  }
}

} // namespace color
} // namespace gos
