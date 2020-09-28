#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gos/color.h>
#include <gos/interpolate.h>

#define GOS_COLOR_UNIT_TEST_COUNT 64

namespace gos {
namespace color {

TEST(GosColorTest, Assign) {
  gos_color c;
  gos_color_rgb rgb;

  gos_color_assign(&c, 0.1, 0.2, 0.3);
  EXPECT_DOUBLE_EQ(0.1, c.a);
  EXPECT_DOUBLE_EQ(0.2, c.b);
  EXPECT_DOUBLE_EQ(0.3, c.c);

  gos_color_assign_rgb(&rgb, 0x07, 0x04, 0x9b); /* Dark blue */
  EXPECT_EQ(0x07, rgb.r);
  EXPECT_EQ(0x04, rgb.g);
  EXPECT_EQ(0x9b, rgb.b);
  gos_color_assign_rgb(&rgb, 0xf4, 0xec, 0x04); /* Yellow */
  EXPECT_EQ(0xf4, rgb.r);
  EXPECT_EQ(0xec, rgb.g);
  EXPECT_EQ(0x04, rgb.b);

  gos_color_assign_rgb32(&rgb, 0x07049b); /* Dark blue */
  EXPECT_EQ(0x07, rgb.r);
  EXPECT_EQ(0x04, rgb.g);
  EXPECT_EQ(0x9b, rgb.b);
  gos_color_assign_rgb32(&rgb, 0xf4ec04); /* Yellow */
  EXPECT_EQ(0xf4, rgb.r);
  EXPECT_EQ(0xec, rgb.g);
  EXPECT_EQ(0x04, rgb.b);
}

TEST(GosColorTest, IsBlack) {
  gos_color c;
  bool rb;

  c.a = c.b = c.c = 0.0;
  rb = gos_color_is_black(&c);
  EXPECT_TRUE(rb);

  c.a = 0.01;
  rb = gos_color_is_black(&c);
  EXPECT_FALSE(rb);

  c.a = -0.01;
  rb = gos_color_is_black(&c);
  EXPECT_TRUE(rb);

  c.a = 0.0;
  c.b = 0.01;
  c.c = 0.0;
  rb = gos_color_is_black(&c);
  EXPECT_FALSE(rb);

  c.a = 0.0;
  c.b = 0.0;
  c.c = 0.01;
  rb = gos_color_is_black(&c);
  EXPECT_FALSE(rb);

  c.a = 0.01;
  c.b = 0.0;
  c.c = 0.01;
  rb = gos_color_is_black(&c);
  EXPECT_FALSE(rb);

  c.a = 0.01;
  c.b = 0.01;
  c.c = 0.01;
  rb = gos_color_is_black(&c);
  EXPECT_FALSE(rb);

  c.a = 1.0;
  c.b = 1.0;
  c.c = 1.0;
  rb = gos_color_is_black(&c);
  EXPECT_FALSE(rb);
}

TEST(GosColorTest, Sum) {
  const int max = 32;
  gos_color c;
  double rd;
  int ai, bi, ci;

  c.a = c.b = c.c = 0.0;
  rd = gos_color_sum(&c);
  EXPECT_DOUBLE_EQ(0.0, rd);

  c.a = 0.01;
  c.b = 0.02;
  c.c = 0.03;
  rd = gos_color_sum(&c);
  EXPECT_DOUBLE_EQ(0.06, rd);

  for (ai = 0; ai < max; ai++) {
    for (bi = 0; bi < max; bi++) {
      for (ci = 0; ci < max; ci++) {
        c.a = (double)ai;
        c.b = (double)bi;
        c.c = (double)ci;
        rd = gos_color_sum(&c);
        EXPECT_DOUBLE_EQ(c.a + c.b + c.c, rd);
      }
    }
  }
}

TEST(GosColorTest, Brightness) {
  const double Gamma = GOS_COLOR_GAMMA;
  gos_color c;
  double br;

  c.a = c.b = c.c = 0.0;
  br = gos_color_brightness(&c, Gamma);
  EXPECT_DOUBLE_EQ(0.0, br);

  c.a = 0.01;
  c.b = 0.02;
  c.c = 0.03;
  br = gos_color_brightness(&c, Gamma);
  EXPECT_DOUBLE_EQ(0.29826688434994880, br);

  //for (ir = 0; ir < maxrgb; ir++) {
  //  for (ig = 0; ig < maxrgb; ig++) {
  //    for (ib = 0; ib < maxrgb; ib++) {
  //    }
  //  }
  //}
}

TEST(GosColorTest, SRGBf) {
  const int Max = 10000;
  int i;
  double y, x, r;

  x = 0.0;
  y = gos_color_s_rgb_f(x);
  EXPECT_DOUBLE_EQ(0.0, y);

  x = 0.0031300;
  y = gos_color_s_rgb_f(x);
  EXPECT_DOUBLE_EQ(0.040439599999999999, y);

  x = 0.0031308;
  y = gos_color_s_rgb_f(x);
  EXPECT_DOUBLE_EQ(0.040449935999999999, y);

  x = 0.0031310;
  y = gos_color_s_rgb_f(x);
  EXPECT_DOUBLE_EQ(0.040452448050838648, y);

  for (i = 0; i < Max; i++) {
    r = (double)i / (double)Max;
    y = gos_color_s_rgb_f(r);
    EXPECT_GE(1.0, y);
    EXPECT_LE(0.0, y);
  }
}

TEST(GosColorTest, SRGBi) {
  const int Max = 256;
  int i;
  double y, x;

  x = 0.0;
  y = gos_color_s_rgb_i(x);
  EXPECT_DOUBLE_EQ(0.0, y);

  x = 5.0;
  y = gos_color_s_rgb_i(x);
  EXPECT_DOUBLE_EQ(0.0015176349177441874, y);

  x = 10.0;
  y = gos_color_s_rgb_i(x);
  EXPECT_DOUBLE_EQ(0.0030352698354883748, y);

  x = 15.0;
  y = gos_color_s_rgb_i(x);
  EXPECT_DOUBLE_EQ(0.0047769534806937292, y);

  for (i = 0; i < Max; i++) {
    y = gos_color_s_rgb_i((double)i);
    EXPECT_GE(1.0, y);
    EXPECT_LE(0.0, y);
  }
}

TEST(GosColorTest, SRGBfa) {
  const int Max = 100;
  double xi, xj, xk, yi, yj, yk;
  int i, j, k;
  gos_color x, y;

  for (i = 0; i < Max; i++) {
    for (j = 0; j < Max; j++) {
      for (k = 0; k < Max; k++) {
        xi = (double)i / (double)Max;
        xj = (double)j / (double)Max;
        xk = (double)k / (double)Max;
        yi = gos_color_s_rgb_f(xi);
        yj = gos_color_s_rgb_f(xj);
        yk = gos_color_s_rgb_f(xk);
        x.a = xi;
        x.b = xj;
        x.c = xk;
        gos_color_s_rgb_fa(&y, &x);
        EXPECT_DOUBLE_EQ(yi, y.a);
        EXPECT_DOUBLE_EQ(yj, y.b);
        EXPECT_DOUBLE_EQ(yk, y.c);
      }
    }
  }
}

TEST(GosColorTest, SRGBia) {
  const int Max = 256;
  double xi, xj, xk, yi, yj, yk;
  int i, j, k;
  gos_color y;
  gos_color_rgb x;

  for (i = 0; i < Max; i++) {
    for (j = 0; j < Max; j++) {
      for (k = 0; k < Max; k++) {
        xi = (double)i;
        xj = (double)j;
        xk = (double)k;
        yi = gos_color_s_rgb_i(xi);
        yj = gos_color_s_rgb_i(xj);
        yk = gos_color_s_rgb_i(xk);
        x.r = i;
        x.g = j;
        x.b = k;
        gos_color_s_rgb_ia(&y, &x);
        EXPECT_DOUBLE_EQ(yi, y.a);
        EXPECT_DOUBLE_EQ(yj, y.b);
        EXPECT_DOUBLE_EQ(yk, y.c);
      }
    }
  }
}

TEST(GosColorTest, Normalize) {
  const int Max = 256;
  int i, j, k;
  double n;
  gos_color c;
  gos_color_rgb rgb;

  for (i = 0; i < Max; i++) {
    n = GOS_COLOR_NORMALIZE(i);
    EXPECT_GE(1.0, n);
    EXPECT_LE(0.0, n);
  }
  for (i = 0; i < Max; i++) {
    for (j = 0; j < Max; j++) {
      for (k = 0; k < Max; k++) {
        rgb.r = i;
        rgb.g = j;
        rgb.b = k;
        gos_color_normalize_rgb(&c, &rgb);
        n = GOS_COLOR_NORMALIZE(i);
        EXPECT_DOUBLE_EQ(n, c.a);
        n = GOS_COLOR_NORMALIZE(j);
        EXPECT_DOUBLE_EQ(n, c.b);
        n = GOS_COLOR_NORMALIZE(k);
        EXPECT_DOUBLE_EQ(n, c.c);
      }
    }
  }
}

TEST(GosColorTest, Denormalize) {
  const int Max = 10;
  int i, j, k, n;
  double r, ri, rj, rk;
  uint8_t u;
  gos_color c;
  gos_color_rgb rgb;

  for (i = 0; i < Max; i++) {
    r = (double)i / (double)Max;
    n = GOS_COLOR_DENORMALIZE(r);
    EXPECT_GE(255, n);
    EXPECT_LE(0, n);
  }
  for (i = 0; i < Max; i++) {
    for (j = 0; j < Max; j++) {
      for (k = 0; k < Max; k++) {
        ri = (double)i / (double)Max;
        rj = (double)j / (double)Max;
        rk = (double)k / (double)Max;
        c.a = ri;
        c.b = rj;
        c.c = rk;
        gos_color_denormalize_rgb(&rgb, &c);
        u = GOS_COLOR_DENORMALIZE(ri);
        EXPECT_EQ(u, rgb.r);
        u = GOS_COLOR_DENORMALIZE(rj);
        EXPECT_EQ(u, rgb.g);
        u = GOS_COLOR_DENORMALIZE(rk);
        EXPECT_EQ(u, rgb.b);
      }
    }
  }
}

TEST(GosColorTest, Interpolate) {
  const int Max = 1000;
  int i;
  double r, y;
  gos_color co, c1, c2;
  c1.a = 0.03;
  c1.b = 0.06;
  c1.c = 0.11;
  c2.a = 0.68;
  c2.b = 0.79;
  c2.c = 0.93;
  for (i = 0; i < Max; i++) {
    r = (double)i / (double)Max;
    gos_color_interpolate_linear(&co, &c1, &c2, r);
    y = gos_interpolate_linear(c1.a, c2.a, r);
    EXPECT_DOUBLE_EQ(y, co.a);
    y = gos_interpolate_linear(c1.b, c2.b, r);
    EXPECT_DOUBLE_EQ(y, co.b);
    y = gos_interpolate_linear(c1.c, c2.c, r);
    EXPECT_DOUBLE_EQ(y, co.c);
  }
}

TEST(GosColorTest, PerceptualSteps) {
  const double Gamma = GOS_COLOR_GAMMA;
  const int Count = GOS_COLOR_UNIT_TEST_COUNT;
  gos_color_rgb** c;
  gos_color_rgb** ca;
  gos_color_rgb c1, c2;
  bool go = true;
  int i;

  /* Dark blue */
  c1.r = 0x07;
  c1.g = 0x04;
  c1.b = 0x9b;

  /* Yellow */
  c2.r = 0xf4;
  c2.g = 0xec;
  c2.b = 0x04;

  c = (gos_color_rgb**)calloc(Count, sizeof(gos_color_rgb*));
  EXPECT_FALSE(c == NULL);

  if (c != NULL) {
    for (i = 0; i < Count; i++) {
      c[i] = (gos_color_rgb*)calloc(1, sizeof(gos_color_rgb));
      EXPECT_FALSE(c[i] == NULL);
      if (c[i] == NULL) {
        go = false;
        break;
      }
    }

    if (go) {
      gos_color_perceptual_steps(c, &c1, &c2, Gamma, Count);
      ca = c;
      for (i = 0; i < Count; i++) {
        EXPECT_LE(c1.r, (*ca)->r);
        EXPECT_GE(c2.r, (*ca)->r);

        EXPECT_LE(c1.g, (*ca)->g);
        EXPECT_GE(c2.g, (*ca)->g);

        EXPECT_LE(c2.b, (*ca)->b);
        EXPECT_GE(c1.b, (*ca)->b);
        ca++;
      }
    }

    for (i = 0; i < Count; i++) {
      if (c[i] != NULL) {
        free(c[i]);
      }
    }

    free(c);
  }
}

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
