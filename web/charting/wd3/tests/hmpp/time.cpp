#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <modules/hmpp/time.h>

namespace wd3 {
namespace test {
namespace time {

TEST(Wd3HmppTimeTest, Parse) {
  wd3::type::time time, now;
  std::string text;

  now = wd3::type::clock::now();

  text = "1971-09-17 23:30:23.1234";
  time = wd3::time::parse(text);
  EXPECT_GT(now, time);

  text = "2666-01-01 00:00:00";
  time = wd3::time::parse(text);
  EXPECT_LT(now, time);
}

TEST(Wd3HmppTimeTest, Format) {
  wd3::type::time time;
  std::string text, original;
  size_t pos;

  EXPECT_TRUE(text.empty());

  time = wd3::type::clock::now();
  text = wd3::time::format(time);
  EXPECT_FALSE(text.empty());

  original = "1971-09-17 23:30:23.1234";
  time = wd3::time::parse(original);
  text = wd3::time::format(time);

  pos = text.find(original);
  EXPECT_EQ(0, pos);
}

} // namespace time
} // namespace test
} // namespace wd3
