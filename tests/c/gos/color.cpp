#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/color.h>

namespace gos {
namespace color {

TEST(GosColorTest, HSL) {
  const double step = 0.01;
  const uint8_t maxrgb = 0xff;
  uint8_t r, g, b, cr, cg, cb;
  double h, s, l, ch, cs, cl;
  for (r = 0; r < maxrgb; r++) {
    for (g = 0; g < maxrgb; g++) {
      for (b = 0; b < maxrgb; b++) {
        gos_color_rgb_to_hsl(r, g, b, &h, &s, &l);
        gos_color_hsl_to_rgb(h, s, l, &cr, &cg, &cb);
        EXPECT_EQ(r, cr);
        EXPECT_EQ(g, cg);
        EXPECT_EQ(b, cb);
      }
    }
  }
  for (h = 0.0; h < 1.0; h += step) {
    for (s = 0.0; s < 1.0; s += step) {
      for (l = 0.0; l < 1.0; l += step) {
        gos_color_hsl_to_rgb(h, s, l, &r, &g, &b);
        gos_color_rgb_to_hsl(r, g, b, &ch, &cs, &cl);
        EXPECT_DOUBLE_EQ(h, ch);
        EXPECT_DOUBLE_EQ(s, cs);
        EXPECT_DOUBLE_EQ(l, cl);
      }
    }
  }
}

TEST(GosColorTest, HSV) {
  const double step = 0.01;
  const uint8_t maxrgb = 0xff;
  uint8_t r, g, b, cr, cg, cb;
  double h, s, v, ch, cs, cv;
  for (r = 0; r < maxrgb; r++) {
    for (g = 0; g < maxrgb; g++) {
      for (b = 0; b < maxrgb; b++) {
        gos_color_rgb_to_hsv(r, g, b, &h, &s, &v);
        gos_color_hsv_to_rgb(h, s, v, &cr, &cg, &cb);
        EXPECT_EQ(r, cr);
        EXPECT_EQ(g, cg);
        EXPECT_EQ(b, cb);
      }
    }
  }
  for (h = 0.0; h < 1.0; h += step) {
    for (s = 0.0; s < 1.0; s += step) {
      for (v = 0.0; v < 1.0; v += step) {
        gos_color_hsv_to_rgb(h, s, v, &r, &g, &b);
        gos_color_rgb_to_hsv(r, g, b, &ch, &cs, &cv);
        EXPECT_DOUBLE_EQ(h, ch);
        EXPECT_DOUBLE_EQ(s, cs);
        EXPECT_DOUBLE_EQ(v, cv);
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
  EXPECT_DOUBLE_EQ(1.0, h);
  EXPECT_DOUBLE_EQ(1.0, s);
  EXPECT_DOUBLE_EQ(1.0, l);
}

TEST(GosColorTest, RGB2HSV) {
  double h, s, v;

  gos_color_rgb_to_hsv(0x00, 0x00, 0x00, &h, &s, &v);
  EXPECT_DOUBLE_EQ(0.0, h);
  EXPECT_DOUBLE_EQ(0.0, s);
  EXPECT_DOUBLE_EQ(0.0, v);

  gos_color_rgb_to_hsv(0xff, 0xff, 0xff, &h, &s, &v);
  EXPECT_DOUBLE_EQ(1.0, h);
  EXPECT_DOUBLE_EQ(1.0, s);
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
  EXPECT_EQ(0xff, g);
  EXPECT_EQ(0xff, b);
}

TEST(GosColorTest, Hue2RGB) {
  uint8_t value;

  value = gos_color_hue_to_rgb(0.0, 0.0, 0.0);
  EXPECT_EQ(0, value);

  value = gos_color_hue_to_rgb(1.0, 1.0, 1.0);
  EXPECT_EQ(1, value);
}

} // namespace color
} // namespace gos
