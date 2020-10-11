#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/types.h>
#include <tiera/tiera.h>

#define TIER_A_UNIT_TEST_COUNT 256
#define TIER_A_UNIT_TEST_MAJOR 8

namespace gos {
namespace expa {
namespace tiera {

static void initialize(gos_tier_a_d* dst, const int& count);
static void shutdown(gos_tier_a_d* dst);

TEST(GosExpaTieraTest, Generate) {
  const int major = TIER_A_UNIT_TEST_MAJOR;
  int count = TIER_A_UNIT_TEST_COUNT;

  double data, depth;

  gos_tier_a_d data_st;

  gos_generation_1d datageneration, depthgeneration;

  bool result;

  datageneration.range.from = 11.0;
  datageneration.range.to = 93.0;
  datageneration.noise = 0.0;

  depthgeneration.range.from = 3200.0;
  depthgeneration.range.to = 9000.0;
  depthgeneration.noise = 0.0;

  initialize(&data_st, count);
  result = gos_tier_a_generate(
    &data_st,
    &datageneration,
    &depthgeneration,
    major);
  ASSERT_TRUE(result);
  for (int i = 0; i < data_st.count; i++) {
    data = data_st.data[i];
    depth = data_st.depth[i];
    EXPECT_LE(datageneration.range.from, data);
    EXPECT_GE(datageneration.range.to, data);
    EXPECT_LE(depthgeneration.range.from, depth);
    EXPECT_GE(depthgeneration.range.to, depth);
  }
  shutdown(&data_st);

  count += 3;

  initialize(&data_st, count);
  result = gos_tier_a_generate(
    &data_st,
    &datageneration,
    &depthgeneration,
    major);
  ASSERT_TRUE(result);
  for (int i = 0; i < data_st.count; i++) {
    data = data_st.data[i];
    depth = data_st.depth[i];
    EXPECT_LE(datageneration.range.from, data);
    EXPECT_GE(datageneration.range.to, data);
    EXPECT_LE(depthgeneration.range.from, depth);
    EXPECT_GE(depthgeneration.range.to, depth);
  }
  shutdown(&data_st);
}

void initialize(gos_tier_a_d* dst, const int& count) {
  dst->count = count;
  dst->data = (double*)malloc(count * sizeof(double));
  dst->depth = (double*)malloc(count * sizeof(double));
  EXPECT_FALSE(dst->data == NULL);
  EXPECT_FALSE(dst->depth == NULL);
}

void shutdown(gos_tier_a_d* dst) {
  free(dst->data);
  free(dst->depth);
  dst->data = NULL;
  dst->depth = NULL;
}

} // namespace tiera
} // namespace expa
} // namespace gos
