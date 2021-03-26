#include <cmath>

#include <memory>
#include <chrono>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <modules/hmpp/types.h>
#include <modules/hmpp/point.h>
#include <modules/hmpp/column.h>

namespace wd3 {
namespace test {
namespace column {

typedef std::unique_ptr<wd3::column> ColumnPtr;

TEST(Wd3HmppColumnTest, Constructors) {
  double depth, value;
  wd3::type::time time;
  int i, size;
  ColumnPtr column, copy;

  size = 4;
  time = wd3::type::clock::now();

  column = std::make_unique<wd3::column>(size, time);
  EXPECT_TRUE((bool)column);
  EXPECT_EQ(size, column->size());
  EXPECT_EQ(time, column->time());

  depth = 100.0;
  value = 10.0;
  for (i = 0; i < size; i++) {
    EXPECT_FALSE(column->is(i));
    column->set(i, depth, value);
    EXPECT_TRUE(column->is(i));
    wd3::point point = column->at(i);
    EXPECT_DOUBLE_EQ(depth, point.depth());
    EXPECT_DOUBLE_EQ(value, point.value());
    depth += 10.0;
    value += 1.0;
  }
}

TEST(Wd3HmppColumnTest, Compare) {
  wd3::type::time time;
  int size;

  size = 4;

  time = wd3::type::clock::now();
  wd3::column columna(size, time);
  wd3::column columnb(size, time);
  EXPECT_TRUE(columna == columnb);
  EXPECT_FALSE(columna != columnb);

  time -= std::chrono::minutes{ 10 };
  wd3::column columnc(size, time);
  EXPECT_TRUE(columnc < columna);
  EXPECT_FALSE(columnc > columna);
}

TEST(Wd3HmppColumnTest, Determination) {
  double depth, value, result;
  wd3::type::time time;
  int i, size;

  size = 4;
  time = wd3::type::clock::now();

  wd3::column column(size, time);
  depth = 100.0;
  value = 10.0;
  for (i = 0; i < size - 1; i++) {
    column.set(i, depth, value);
    depth += 10.0;
    value += 1.0;
  }

  result = column.determination(99.0);
  EXPECT_FALSE(std::isfinite(result));

  result = column.determination(100.0);
  EXPECT_TRUE(std::isfinite(result));
  EXPECT_DOUBLE_EQ(10.0, result);

  result = column.determination(105.0);
  EXPECT_TRUE(std::isfinite(result));
  EXPECT_DOUBLE_EQ(10.5, result);

  result = column.determination(115.0);
  EXPECT_TRUE(std::isfinite(result));
  EXPECT_DOUBLE_EQ(11.5, result);

  result = column.determination(125.0);
  EXPECT_FALSE(std::isfinite(result));

  column.set(size - 1, depth, value);
  result = column.determination(125.0);
  EXPECT_TRUE(std::isfinite(result));
  EXPECT_DOUBLE_EQ(12.5, result);
}

TEST(Wd3HmppColumnTest, Sort) {
  const int size = 4;
  wd3::type::time time = wd3::type::clock::now();
  wd3::column column(size, time);

  column.set(0, 30.0, 3.0);
  column.set(1, 10.0, 1.0);
  column.set(3, 20.0, 2.0);

  column.sort();

  double depth = 0.0;
  double value = 0.0;
  for (int i = 0; i < size - 1; i++) {
    EXPECT_TRUE(column.is(i));
    wd3::point point = column.at(i);
    EXPECT_LT(depth, point.depth());
    EXPECT_LT(value, point.value());
    depth = point.depth();
    value = point.value();
  }
  EXPECT_FALSE(column.is(size - 1));
}

} // namespace column
} // namespace test
} // namespace wd3
