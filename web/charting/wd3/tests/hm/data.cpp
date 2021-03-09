#include <cstdbool>
#include <cmath>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/scale.h>

#include <modules/hm/hm.h>

#define WD3_HM_DATA_COUNT       4
#define WD3_HM_SCREEN_WIDTH   200
#define WD3_HM_SCREEN_HEIGHT  100

namespace wd3 {
namespace hm {
namespace data {

TEST(EfdHmTest, DataSet) {
  wd3hm context;
  bool result;

  ghm_initialize(&context, WD3_HM_SCREEN_WIDTH, WD3_HM_SCREEN_HEIGHT);

  context.count = WD3_HM_DATA_COUNT;

  result = ghm_create_data(&context);
  EXPECT_TRUE(result);

  result = ghm_data_set(&context, 0, 1.0, 10.0);
  EXPECT_TRUE(result);

  result = ghm_data_set(&context, 3, 1.0, 10.0);
  EXPECT_TRUE(result);

  result = ghm_data_set(&context, -1, 1.0, 10.0);
  EXPECT_FALSE(result);

  result = ghm_data_set(&context, 4, 1.0, 10.0);
  EXPECT_FALSE(result);

  ghm_shutdown_data(&context);
}

TEST(EfdHmTest, DataCheck) {
  wd3hm context;
  bool result;

  ghm_initialize(&context, WD3_HM_SCREEN_WIDTH, WD3_HM_SCREEN_HEIGHT);

  context.count = WD3_HM_DATA_COUNT;

  result = ghm_create_data(&context);
  EXPECT_TRUE(result);

  ghm_data_set(&context, 0, 1.0, 10.0);
  ghm_data_set(&context, 1, 2.0, 20.0);
  ghm_data_set(&context, 2, 3.0, 30.0);
  ghm_data_set(&context, 3, 4.0, 40.0);

  result = ghm_data_check(&context);
  EXPECT_TRUE(result);

  ghm_data_set(&context, 0, 1.0, 10.0);
  ghm_data_set(&context, 1, 1.0, 10.0);
  ghm_data_set(&context, 2, 1.0, 10.0);
  ghm_data_set(&context, 3, 1.0, 10.0);

  result = ghm_data_check(&context);
  EXPECT_TRUE(result);

  ghm_data_set(&context, 0, 4.0, 10.0);
  ghm_data_set(&context, 1, 3.0, 20.0);
  ghm_data_set(&context, 2, 2.0, 30.0);
  ghm_data_set(&context, 3, 1.0, 40.0);

  result = ghm_data_check(&context);
  EXPECT_FALSE(result);

  ghm_data_set(&context, 0, 1.0, 40.0);
  ghm_data_set(&context, 1, 2.0, 30.0);
  ghm_data_set(&context, 2, 3.0, 20.0);
  ghm_data_set(&context, 3, 4.0, 10.0);

  result = ghm_data_check(&context);
  EXPECT_FALSE(result);

  ghm_data_set(&context, 0, 4.0, 40.0);
  ghm_data_set(&context, 1, 3.0, 30.0);
  ghm_data_set(&context, 2, 2.0, 20.0);
  ghm_data_set(&context, 3, 1.0, 10.0);

  result = ghm_data_check(&context);
  EXPECT_FALSE(result);

  ghm_shutdown_data(&context);
}

TEST(EfdHmTest, PointDetermination) {
  bool bresult;
  wd3hm context;
  double y, value, result;

  ghm_initialize(&context, WD3_HM_SCREEN_WIDTH, WD3_HM_SCREEN_HEIGHT);

  context.count = WD3_HM_DATA_COUNT;

  bresult = ghm_create_data(&context);
  EXPECT_TRUE(bresult);

  context.yscale.range.from = (double)(context.screen.height);

  ghm_data_set(&context, 0, 10.0, 1.0);
  ghm_data_set(&context, 1, 20.0, 2.0);
  ghm_data_set(&context, 2, 30.0, 3.0);
  ghm_data_set(&context, 3, 40.0, 4.0);

  ghm_domain_y_set(&context, 5.0, 45.0);

  value = 0.0;
  result = ghm_point_determination(&context, value);
  EXPECT_FALSE(std::isfinite(result));

  value = 20.0;
  y = gos_scale_reverse(&context.yscale, value);
  result = ghm_point_determination(&context, value);
  EXPECT_TRUE(std::isfinite(result));
  EXPECT_DOUBLE_EQ(y / 10.0, result);

  value = 40.0;
  y = gos_scale_reverse(&context.yscale, value);
  result = ghm_point_determination(&context, value);
  EXPECT_TRUE(std::isfinite(result));
  EXPECT_DOUBLE_EQ(y / 10.0, result);

  value = 70.0;
  y = gos_scale_reverse(&context.yscale, value);
  result = ghm_point_determination(&context, value);
  EXPECT_TRUE(std::isfinite(result));
  EXPECT_DOUBLE_EQ(y / 10.0, result);

  value = 90.0;
  result = ghm_point_determination(&context, value);
  EXPECT_FALSE(std::isfinite(result));

  value = 100.0;
  result = ghm_point_determination(&context, value);
  EXPECT_FALSE(std::isfinite(result));

  ghm_shutdown_data(&context);
}

TEST(EfdHmTest, Buffer) {
  int i, j;
  bool result;
  double x, y;
  wd3hm context;

  ghm_initialize(&context, WD3_HM_SCREEN_WIDTH, WD3_HM_SCREEN_HEIGHT);

  context.count = WD3_HM_DATA_COUNT;

  result = ghm_create_data(&context);
  EXPECT_TRUE(result);
  result = ghm_create_buffer(&context);
  EXPECT_TRUE(result);

  for (i = 0; i < WD3_HM_SCREEN_WIDTH; i++) {
    for (j = 0; j < context.count; j++) {
      x = ((double)i) * ((double)j);
      y = x * 10.0;
      ghm_data_set(&context, j, x, y);
    }
    ghm_buffer_insert(&context, i);
  }

  for (i = 0; i < WD3_HM_SCREEN_WIDTH; i++) {
    for (j = 0; j < context.count; j++) {
      x = ((double)i) * ((double)j);
      y = x * 10.0;
      EXPECT_DOUBLE_EQ(x, context.buffer[j][i].x);
      EXPECT_DOUBLE_EQ(y, context.buffer[j][i].y);
    }
  }

  ghm_rescale(&context);
  EXPECT_DOUBLE_EQ(0.0, context.yscale.domain.from);
  EXPECT_DOUBLE_EQ(x, context.yscale.domain.to);
  EXPECT_DOUBLE_EQ(0.0, context.zscale.domain.from);
  EXPECT_DOUBLE_EQ(y, context.zscale.domain.to);

  ghm_buffer_shift(&context);

  for (i = 1; i < WD3_HM_SCREEN_WIDTH; i++) {
    for (j = 0; j < context.count; j++) {
      x = ((double)i) * ((double)j);
      y = x * 10.0;
      EXPECT_DOUBLE_EQ(x, context.buffer[j][i - 1].x);
      EXPECT_DOUBLE_EQ(y, context.buffer[j][i - 1].y);
    }
  }

  ghm_rescale(&context);
  EXPECT_DOUBLE_EQ(0.0, context.yscale.domain.from);
  EXPECT_DOUBLE_EQ(x, context.yscale.domain.to);
  EXPECT_DOUBLE_EQ(0.0, context.zscale.domain.from);
  EXPECT_DOUBLE_EQ(y, context.zscale.domain.to);

  ghm_shutdown_buffer(&context);
  ghm_shutdown_data(&context);
}

} // namespace data
} // namespace hm
} // namespace wd3
