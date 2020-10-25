#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wasm/performance.h>

#define GOS_EXPA_WASM_PERFORMANCE_SIZE 2
#define GOS_EXPA_WASM_PERFORMANCE_TEST_BUFFER_SIZE 1024

namespace gos {
namespace expa {
namespace performance {

TEST(GosExpaWasmPerformanceTest, Initialize) {
  bool result;
  gos_performance performance;

  gos_performance_initialize(&performance);
  EXPECT_EQ(0UL, performance.count);
  EXPECT_EQ(0.0, performance.sum);
  EXPECT_EQ(0.0, performance.value);
  EXPECT_LT(0.0, performance.minimum);
  EXPECT_GT(0.0, performance.maximum);
  EXPECT_EQ(0.0, performance.average);
  EXPECT_EQ(0.0, performance.sd);
  EXPECT_FALSE(performance.is);

  /* Item array */
  EXPECT_TRUE(performance.items.items == NULL);
  EXPECT_EQ(0, performance.items.used);
  EXPECT_EQ(0, performance.items.size);

  /* At time spec structure */
  EXPECT_EQ(0, performance.at.tv_sec);
  EXPECT_EQ(0L, performance.at.tv_nsec);

  gos_performance_shutdown(&performance);

  result = gos_performance_initialize_sd(
    &performance, GOS_EXPA_WASM_PERFORMANCE_SIZE);
  EXPECT_TRUE(result);
  EXPECT_EQ(0UL, performance.count);
  EXPECT_EQ(0.0, performance.sum);
  EXPECT_EQ(0.0, performance.value);
  EXPECT_LT(0.0, performance.minimum);
  EXPECT_GT(0.0, performance.maximum);
  EXPECT_EQ(0.0, performance.average);
  EXPECT_EQ(0.0, performance.sd);
  EXPECT_FALSE(performance.is);

  /* Item array */
  EXPECT_FALSE(performance.items.items == NULL);
  EXPECT_EQ(0, performance.items.used);
  EXPECT_EQ(GOS_EXPA_WASM_PERFORMANCE_SIZE, performance.items.size);

  /* At time spec structure */
  EXPECT_EQ(0, performance.at.tv_sec);
  EXPECT_EQ(0L, performance.at.tv_nsec);

  gos_performance_shutdown(&performance);
}

TEST(GosExpaWasmPerformanceTest, Update) {
  int i;
  bool result;
  gos_performance performance;

  gos_performance_initialize(&performance);

  for (i = 0; i < 10; i++) {
    gos_performance_update(&performance, (double)i);
  }
  EXPECT_EQ(10UL, performance.count);
  EXPECT_DOUBLE_EQ(45.0, performance.sum);
  EXPECT_DOUBLE_EQ(9.0, performance.value);
  EXPECT_DOUBLE_EQ(0.0, performance.minimum);
  EXPECT_DOUBLE_EQ(9.0, performance.maximum);

  gos_performance_shutdown(&performance);

  gos_performance_initialize_sd(
    &performance, GOS_EXPA_WASM_PERFORMANCE_SIZE);

  for (i = 0; i < 10; i++) {
    result = gos_performance_update_sd(&performance, (double)i);
    EXPECT_TRUE(result);
  }
  EXPECT_EQ(10, performance.items.used);

  gos_performance_shutdown(&performance);
}

TEST(GosExpaWasmPerformanceTest, Reset) {
  int i;
  bool result;
  gos_performance performance;

  gos_performance_initialize_sd(
    &performance, GOS_EXPA_WASM_PERFORMANCE_SIZE);
  for (i = 0; i < 10; i++) {
    result = gos_performance_update_sd(&performance, (double)i);
    EXPECT_TRUE(result);
  }

  gos_performance_reset_sd(&performance);
  EXPECT_FALSE(performance.items.items == NULL);
  EXPECT_EQ(0, performance.items.used);

  gos_performance_shutdown(&performance);
}

TEST(GosExpaWasmPerformanceTest, Format) {
  int result;
  gos_performance performance;
  char pfb[GOS_EXPA_WASM_PERFORMANCE_TEST_BUFFER_SIZE];

  gos_performance_initialize(&performance);

  performance.average = 1.1;
  performance.sd = 2.2;
  performance.minimum = 3.3;
  performance.maximum = 4.4;
  performance.count = 418;

  result = gos_performance_format(
    &performance, pfb, GOS_EXPA_WASM_PERFORMANCE_TEST_BUFFER_SIZE);
  EXPECT_EQ(67, result);
  EXPECT_STREQ(
    "Average: 1.100000, Minimum: 3.300000, Maximum: 4.400000, Count: 418",
    pfb);

  result = gos_performance_format_sd(
    &performance, pfb, GOS_EXPA_WASM_PERFORMANCE_TEST_BUFFER_SIZE);
  EXPECT_EQ(81, result);
  EXPECT_STREQ(
    "Average: 1.100000, SD: 2.200000, Minimum: 3.300000, Maximum: 4.400000, Count: 418",
    pfb);
}

}
}
}
