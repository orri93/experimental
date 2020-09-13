#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/types.h>
#include <gos/signal.h>

#define GOS_SIGNAL_UNIT_TEST_COUNT 12

namespace gos {
namespace signal {

TEST(GosSignalTest, RandomRange) {
  gos_range_1d range;
  double v;

  srand(1); /* Fixed random output */

  range.from = 2.0;
  range.to = 8.0;

  for (int i = 0; i < 1000; i++) {
    v = gos_signal_random_range(&range);
    EXPECT_LE(range.from, v);
    EXPECT_GE(range.to, v);
  }
}

TEST(GosSignalTest, GenerateRange) {
  const int c = GOS_SIGNAL_UNIT_TEST_COUNT;
  gos_point_2d v[GOS_SIGNAL_UNIT_TEST_COUNT];
  gos_range_2d range;

  srand(1); /* Fixed random output */

  range.first.from = 1.0;
  range.first.to = 32.0;
  range.second.from = 2.0;
  range.second.to = 8.0;

  gos_signal_generate_2d_range_random_x_and_y(v, &range, c);

  for (int i = 0; i < c; i++) {
    double x = v[i].x;
    double y = v[i].y;
    EXPECT_LE(range.first.from, x);
    EXPECT_GE(range.first.to, x);
    EXPECT_LE(range.second.from, y);
    EXPECT_GE(range.second.to, y);
  }
}

} // namespace signal
} // namespace gos
