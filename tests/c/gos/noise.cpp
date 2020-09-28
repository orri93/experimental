#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/noise.h>

namespace gos {
namespace color {

TEST(GosNoiseTest, Random) {
  const int Count = 1024;

  int n, m;
  long sum = 0;
  for (int x = 0; x < Count; x++) {
    n = gos_noise_random_maximum_integer(1024);
    EXPECT_LE(0, n);
    EXPECT_GE(1024, n);
    sum += n;
  }
  m = sum / Count;
  EXPECT_LE(256, m);
  EXPECT_GE(768, m);
}

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
