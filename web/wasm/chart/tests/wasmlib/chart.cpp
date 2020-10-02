#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/chart.h>

namespace gos {
namespace chart {

TEST(GosChartTest, CreateGradient) {
  bool callresult;

  gos_rgb_gradient gradient;

  callresult = gos_chart_create_gradient(&gradient);
  EXPECT_TRUE(callresult);

  gos_color_free_rgb_gradient(&gradient);
}

}
}
