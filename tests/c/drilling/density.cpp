#include <cstdlib>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/types.h>
#include <gos/drilling/density.h>

#define GOS_DRILLING_DENSITY_UNIT_TEST_COUNT 256
#define GOS_DRILLING_DENSITY_UNIT_TEST_MAJOR 8

namespace gos {
namespace drilling {
namespace density {

static void initialize(gos_drilling_density* ecd, const int& count);
static void shutdown(gos_drilling_density* ecd);

TEST(GosDrillingDensityTest, Generate) {
  const int major = GOS_DRILLING_DENSITY_UNIT_TEST_MAJOR;
  int count = GOS_DRILLING_DENSITY_UNIT_TEST_COUNT;

  double density, depth;

  gos_drilling_density ecd;

  gos_generation_1d densitygeneration, depthgeneration;

  bool result;

  densitygeneration.range.from = 11.0;
  densitygeneration.range.to = 93.0;
  densitygeneration.noise = 0.0;

  depthgeneration.range.from = 3200.0;
  depthgeneration.range.to = 9000.0;
  depthgeneration.noise = 0.0;

  initialize(&ecd, count);
  result = gos_exp_drilling_density_generate(
    &ecd,
    &densitygeneration,
    &depthgeneration,
    major);
  ASSERT_TRUE(result);
  for (int i = 0; i < ecd.count; i++) {
    density = ecd.density[i];
    depth = ecd.depth[i];
    EXPECT_LE(densitygeneration.range.from, density);
    EXPECT_GE(densitygeneration.range.to, density);
    EXPECT_LE(depthgeneration.range.from, depth);
    EXPECT_GE(depthgeneration.range.to, depth);
  }
  shutdown(&ecd);

  count += 3;

  initialize(&ecd, count);
  result = gos_exp_drilling_density_generate(
    &ecd,
    &densitygeneration,
    &depthgeneration,
    major);
  ASSERT_TRUE(result);
  for (int i = 0; i < ecd.count; i++) {
    density = ecd.density[i];
    depth = ecd.depth[i];
    EXPECT_LE(densitygeneration.range.from, density);
    EXPECT_GE(densitygeneration.range.to, density);
    EXPECT_LE(depthgeneration.range.from, depth);
    EXPECT_GE(depthgeneration.range.to, depth);
  }
  shutdown(&ecd);
}

void initialize(gos_drilling_density* ecd, const int& count) {
  ecd->count = count;
  ecd->density = (double*)malloc(count * sizeof(double));
  ecd->depth = (double*)malloc(count * sizeof(double));
  EXPECT_FALSE(ecd->density == NULL);
  EXPECT_FALSE(ecd->depth == NULL);
}

void shutdown(gos_drilling_density* ecd) {
  free(ecd->density);
  free(ecd->depth);
  ecd->density = NULL;
  ecd->depth = NULL;
}

} // namespace density
} // namespace drilling
} // namespace gos
