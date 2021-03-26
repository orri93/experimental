#include <memory>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <modules/hmpp/point.h>

namespace wd3 {
namespace test {
namespace point {

TEST(Wd3HmppPointTest, Constructors) {
  double depth, value;
  wd3::PointPtr point, copy;

  depth = 56.78;
  value = 12.34;

  point = std::make_unique<wd3::point>(depth, value);
  EXPECT_TRUE((bool)point);
  EXPECT_DOUBLE_EQ(depth, point->depth());
  EXPECT_DOUBLE_EQ(value, point->value());

  copy = std::make_unique<wd3::point>(*point);
  EXPECT_TRUE((bool)copy);
  EXPECT_NE(point.get(), copy.get());

  EXPECT_DOUBLE_EQ(point->depth(), copy->depth());
  EXPECT_DOUBLE_EQ(point->value(), copy->value());

  wd3::point assigned = *point;
  EXPECT_DOUBLE_EQ(point->depth(), assigned.depth());
  EXPECT_DOUBLE_EQ(point->value(), assigned.value());
}

} // namespace point
} // namespace test
} // namespace wd3
