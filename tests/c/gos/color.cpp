#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/color.h>

namespace gos {
namespace color {

TEST(GosColorTest, HSL) {
  const int maxrgb = 256;
  const double step = 0.01;
  int ir, ig, ib;
  double h, s, l;
  uint8_t r, g, b, cr, cg, cb;
  for (ir = 0; ir < maxrgb; ir++) {
    for (ig = 0; ig < maxrgb; ig++) {
      for (ib = 0; ib < maxrgb; ib++) {
        r = (uint8_t)ir;
        g = (uint8_t)ig;
        b = (uint8_t)ib;
        gos_color_rgb_to_hsl(r, g, b, &h, &s, &l);
        gos_color_hsl_to_rgb(h, s, l, &cr, &cg, &cb);
        EXPECT_EQ(r, cr);
        EXPECT_EQ(g, cg);
        EXPECT_EQ(b, cb);
      }
    }
  }
}

TEST(GosColorTest, HSV) {
  const int maxrgb = 256;
  const double step = 0.01;
  int ir, ig, ib;
  double h, s, v;
  uint8_t r, g, b, cr, cg, cb;
  for (ir = 0; ir < maxrgb; ir++) {
    for (ig = 0; ig < maxrgb; ig++) {
      for (ib = 0; ib < maxrgb; ib++) {
        r = (uint8_t)ir;
        g = (uint8_t)ig;
        b = (uint8_t)ib;
        gos_color_rgb_to_hsv(r, g, b, &h, &s, &v);
        gos_color_hsv_to_rgb(h, s, v, &cr, &cg, &cb);
        EXPECT_EQ(r, cr);
        EXPECT_EQ(g, cg);
        EXPECT_EQ(b, cb);
      }
    }
  }
}

TEST(GosColorTest, RGB2HSL) {
  double h, s, l;

  gos_color_rgb_to_hsl(0x00, 0x00, 0x00, &h, &s, &l);
  EXPECT_DOUBLE_EQ(0.0, h);
  EXPECT_DOUBLE_EQ(0.0, s);
  EXPECT_DOUBLE_EQ(0.0, l);

  gos_color_rgb_to_hsl(0xff, 0xff, 0xff, &h, &s, &l);
  EXPECT_DOUBLE_EQ(0.0, h);
  EXPECT_DOUBLE_EQ(0.0, s);
  EXPECT_DOUBLE_EQ(1.0, l);
}

TEST(GosColorTest, RGB2HSV) {
  double h, s, v;

  gos_color_rgb_to_hsv(0x00, 0x00, 0x00, &h, &s, &v);
  EXPECT_DOUBLE_EQ(0.0, h);
  EXPECT_DOUBLE_EQ(0.0, s);
  EXPECT_DOUBLE_EQ(0.0, v);

  gos_color_rgb_to_hsv(0xff, 0xff, 0xff, &h, &s, &v);
  EXPECT_DOUBLE_EQ(0.0, h);
  EXPECT_DOUBLE_EQ(0.0, s);
  EXPECT_DOUBLE_EQ(1.0, v);
}

TEST(GosColorTest, HSL2RGB) {
  uint8_t r, g, b;

  gos_color_hsl_to_rgb(0.0, 0.0, 0.0, &r, &g, &b);
  EXPECT_EQ(0x00, r);
  EXPECT_EQ(0x00, g);
  EXPECT_EQ(0x00, b);

  gos_color_hsl_to_rgb(1.0, 1.0, 1.0, &r, &g, &b);
  EXPECT_EQ(0xff, r);
  EXPECT_EQ(0xff, g);
  EXPECT_EQ(0xff, b);
}

TEST(GosColorTest, HSV2RGB) {
  uint8_t r, g, b;

  gos_color_hsv_to_rgb(0.0, 0.0, 0.0, &r, &g, &b);
  EXPECT_EQ(0x00, r);
  EXPECT_EQ(0x00, g);
  EXPECT_EQ(0x00, b);

  gos_color_hsv_to_rgb(1.0, 1.0, 1.0, &r, &g, &b);
  EXPECT_EQ(0xff, r);
  EXPECT_EQ(0x00, g);
  EXPECT_EQ(0x00, b);
}

TEST(GosColorTest, Hue2RGB) {
  uint8_t value;

  value = gos_color_hue(0.0, 0.0, 0.0);
  EXPECT_EQ(0, value);

  value = gos_color_hue(1.0, 1.0, 1.0);
  EXPECT_EQ(0xff, value);
}

} // namespace color
} // namespace gos
