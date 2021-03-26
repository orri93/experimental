#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <modules/hmpp/types.h>
#include <modules/hmpp/point.h>
#include <modules/hmpp/column.h>
#include <modules/hmpp/data.h>

#define WD3_TEST_DATA_SORT_VALUE_COUNT 6

namespace wd3 {
namespace test {
namespace data {

TEST(Wd3HmppDataTest, Sort) {
  const int values[WD3_TEST_DATA_SORT_VALUE_COUNT] = {
    2, 1, 6, 4, 0, 5
  };
  const int size = 4;
  double depth, value;
  int i, j;
  wd3::ColumnIterator it;
  wd3::type::time time, now;
  wd3::data data(size);

  now = wd3::type::clock::now();
  for (i = 0; i < WD3_TEST_DATA_SORT_VALUE_COUNT; i++) {
    time = now + std::chrono::minutes{ values[i] };
    wd3::column column(size, time);
    for (j = 0; j < size; j++) {
      depth = 100.0 + 10.0 * static_cast<double>((j + 1) * values[i]);
      value = 10.0 + static_cast<double>((j + 1) * values[i]);
      column.set(j, depth, value);
    }
    data.add(column);
  }

  it = data.first();
  for (i = 0; i < WD3_TEST_DATA_SORT_VALUE_COUNT; i++) {
    EXPECT_TRUE(it != data.last());
    EXPECT_TRUE(it->is(0));
    wd3::point point = it->at(0);
    depth = 100.0 + 10.0 * static_cast<double>(values[i]);
    EXPECT_DOUBLE_EQ(depth, point.depth());
    it++;
  }

  data.sort();
  depth = 0.0;
  it = data.first();
  for (i = 0; i < WD3_TEST_DATA_SORT_VALUE_COUNT; i++) {
    EXPECT_TRUE(it != data.last());
    EXPECT_TRUE(it->is(0));
    wd3::point point = it->at(0);
    EXPECT_LT(depth, point.depth());
    depth = point.depth();
    it++;
  }
}

} // namespace data
} // namespace test
} // namespace wd3
