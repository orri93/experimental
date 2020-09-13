#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/types.h>
#include <gos/geometry.h>

namespace gos {
namespace geometry {

TEST(GosGeometryTest, Distance) {
  gos_range_1d range;
  double distance;

  range.from = 3.0;
  range.to = 12.0;

  distance = gos_geometry_distance_1d(&range);
  EXPECT_DOUBLE_EQ(9.0, distance);
}

} // namespace geometry
} // namespace gos
