#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <gos/color.h>
#include <gos/interpolate.h>

void gos_color_assign(gos_color* co, double a, double b, double c) {
  assert(co != NULL);
  co->a = a;
  co->b = b;
  co->c = c;
}
void gos_color_assign_rgb(gos_color_rgb* co, uint8_t r, uint8_t g, uint8_t b) {
  assert(co != NULL);
  co->r = r;
  co->g = g;
  co->b = b;
}
void gos_color_assign_rgb32(gos_color_rgb* co, uint32_t rgb) {
  gos_color_assign_rgb(
    co,
    (rgb & 0xff0000) >> 16,
    (rgb & 0xff00) >> 8,
    rgb & 0xff);
}

double gos_color_min_f(double a, double b, double c) {
  return fmin(a, fmin(b, c));
}
double gos_color_max_f(double a, double b, double c) {
  return fmax(a, fmax(b, c));
}

bool gos_color_is_black(gos_color* c) {
  assert(c != NULL);
  return c->a <= 0.0 && c->b <= 0.0 && c->c <= 0.0;
}

double gos_color_sum(gos_color* c) {
  assert(c != NULL);
  return c->a + c->b + c->c;
}

double gos_color_brightness(gos_color* c, double gamma) {
  return pow(gos_color_sum(c), gamma);
}

/* Returns a sRGB value in the range [0,1] for linear input in [0,1]. */
double gos_color_s_rgb_f(double x) {
  return x <= 0.0031308 ? x * 12.92 : (1.055 * pow(x, 1.0 / 2.4)) - 0.055;
}

/* Returns a linear value in the range [0,1] for sRGB input in [0,255]. */
double gos_color_s_rgb_i(double x) {
  x /= GOS_COLOR_MAX_RGB_F;
  return x <= 0.04045 ? x / 12.92 : pow((x + 0.055) / 1.055, 2.4);
}

void gos_color_s_rgb_fa(gos_color* c, gos_color* i) {
  assert(c != NULL);
  assert(i != NULL);
  c->a = gos_color_s_rgb_f(i->a);
  c->b = gos_color_s_rgb_f(i->b);
  c->c = gos_color_s_rgb_f(i->c);
}

void gos_color_s_rgb_ia(gos_color* c, gos_color_rgb* rgb) {
  assert(c != NULL);
  assert(rgb != NULL);
  c->a = gos_color_s_rgb_i((double)rgb->r);
  c->b = gos_color_s_rgb_i((double)rgb->g);
  c->c = gos_color_s_rgb_i((double)rgb->b);
}

void gos_color_normalize_rgb(gos_color* normalized, gos_color_rgb* rgb) {
  assert(normalized != NULL);
  assert(rgb != NULL);
  normalized->a = GOS_COLOR_NORMALIZE(rgb->r);
  normalized->b = GOS_COLOR_NORMALIZE(rgb->g);
  normalized->c = GOS_COLOR_NORMALIZE(rgb->b);
}

void gos_color_denormalize_rgb(gos_color_rgb* rgb, gos_color* normalized) {
  assert(normalized != NULL);
  assert(rgb != NULL);
  rgb->r = GOS_COLOR_DENORMALIZE(normalized->a);
  rgb->g = GOS_COLOR_DENORMALIZE(normalized->b);
  rgb->b = GOS_COLOR_DENORMALIZE(normalized->c);
}

void gos_color_interpolate_linear(
  gos_color* co,
  gos_color* c1,
  gos_color* c2,
  double f) {
  assert(co != NULL);
  assert(c1 != NULL);
  assert(c2 != NULL);
  co->a = gos_interpolate_linear(c1->a, c2->a, f);
  co->b = gos_interpolate_linear(c1->b, c2->b, f);
  co->c = gos_interpolate_linear(c1->c, c2->c, f);
}

/* See https://stackoverflow.com/questions/22607043/color-gradient-algorithm */
void gos_color_perceptual_steps(
  gos_color_rgb** crgbo,
  gos_color_rgb* crgb1,
  gos_color_rgb* crgb2,
  double gamma,
  int steps) {
  int i;
  double bright1, bright2, intensity, ratio, sum;
  gos_color s, c, cs1, cs2;
  assert(crgbo != NULL);
  assert(crgb1 != NULL);
  assert(crgb2 != NULL);
  gos_color_s_rgb_ia(&cs1, crgb1);
  gos_color_s_rgb_ia(&cs2, crgb2);
  bright1 = gos_color_brightness(&cs1, gamma);
  bright2 = gos_color_brightness(&cs2, gamma);
  for (i = 0; i < steps; i++) {
    ratio = (double)i / (double)steps;
    intensity = pow(
      gos_interpolate_linear(bright1, bright2, ratio),
      1.0 / gamma);
    gos_color_interpolate_linear(&c, &cs1, &cs2, ratio);
    if (!gos_color_is_black(&c)) {
      sum = gos_color_sum(&c);
      c.a = intensity * c.a / sum;
      c.b = intensity * c.b / sum;
      c.c = intensity * c.c / sum;
    }
    gos_color_s_rgb_fa(&s, &c);
    gos_color_denormalize_rgb(*(crgbo++), &s);
  }
}

void gos_color_rgb_to_hsl(
  uint8_t r, uint8_t g, uint8_t b,
  double* h, double* s, double* l) {
  double minimum, maximum;
  double rd = GOS_COLOR_NORMALIZE(r);
  double gd = GOS_COLOR_NORMALIZE(g);
  double bd = GOS_COLOR_NORMALIZE(b);
#ifdef GOS_COLOR_MIN_MAX_MACRO
  minimum = GOS_COLOR_MIN(rd, gd, bd);
  maximum = GOS_COLOR_MAX(rd, gd, bd);
#else
  minimum = gos_color_min_f(rd, gd, bd);
  maximum = gos_color_max_f(rd, gd, bd);
#endif
  assert(h != NULL);
  assert(s != NULL);
  assert(l != NULL);
  *l = (maximum + minimum) / 2.0;
  if (maximum == minimum) {
    *h = *s = 0.0; // achromatic
  } else {
    double d = maximum - minimum;
    *s = ((*l) > 0.5) ?
      (d / (2.0 - maximum - minimum)) :
      (d / (maximum + minimum));
    if (maximum == rd) {
      *h = (gd - bd) / d + (gd < bd ? 6.0 : 0.0);
    } else if (maximum == gd) {
      *h = (bd - rd) / d + 2.0;
    } else if (maximum == bd) {
      *h = (rd - gd) / d + 4.0;
    }
    *h /= 6.0;
  }
}

void gos_color_rgb_to_hsv(
  uint8_t r, uint8_t g, uint8_t b,
  double* h, double* s, double* v) {
  double minimum, maximum;
  double rd = GOS_COLOR_NORMALIZE(r);
  double gd = GOS_COLOR_NORMALIZE(g);
  double bd = GOS_COLOR_NORMALIZE(b);
#ifdef GOS_COLOR_MIN_MAX_MACRO
  maximum = GOS_COLOR_MAX(rd, gd, bd);
  minimum = GOS_COLOR_MIN(rd, gd, bd);
#else
  minimum = gos_color_min_f(rd, gd, bd);
  maximum = gos_color_max_f(rd, gd, bd);
#endif
  assert(h != NULL);
  assert(s != NULL);
  assert(v != NULL);
  *v = maximum;
  double d = maximum - minimum;
  *s = maximum == 0 ? 0 : d / maximum;
  if (maximum == minimum) {
    *h = 0.0; // achromatic
  } else {
    if (maximum == rd) {
      *h = (gd - bd) / d + (gd < bd ? 6.0 : 0.0);
    } else if (maximum == gd) {
      *h = (bd - rd) / d + 2.0;
    } else if (maximum == bd) {
      *h = (rd - gd) / d + 4.0;
    }
    *h /= 6.0;
  }
}

void gos_color_hsl_to_rgb(
  double h, double s, double l,
  uint8_t* r, uint8_t* g, uint8_t* b) {
  assert(r != NULL);
  assert(g != NULL);
  assert(b != NULL);
  if (s == 0.0) {
    *r = *g = *b = GOS_COLOR_DENORMALIZE(l); // achromatic
  } else {
    double q = (l < 0.5) ? (l * (1.0 + s)) : (l + s - l * s);
    double p = 2.0 * l - q;
    *r = gos_color_hue(p, q, h + 1.0 / 3.0);
    *g = gos_color_hue(p, q, h);
    *b = gos_color_hue(p, q, h - 1.0 / 3.0);
  }
}

void gos_color_hsv_to_rgb(
  double h, double s, double v,
  uint8_t* r, uint8_t* g, uint8_t* b) {
  int i;
  double f, p, q, t;
  i = (int)(h * 6.0);
  v *= GOS_COLOR_ROUND_RGB_F;
  f = h * 6.0 - i;
  p = v * (1.0 - s);
  q = v * (1.0 - f * s);
  t = v * (1.0 - (1.0 - f) * s);
  assert(r != NULL);
  assert(g != NULL);
  assert(b != NULL);
  switch (i % 6) {
  case 0:
    *r = (uint8_t)v;
    *g = (uint8_t)t;
    *b = (uint8_t)p;
    break;
  case 1:
    *r = (uint8_t)q;
    *g = (uint8_t)v;
    *b = (uint8_t)p;
    break;
  case 2:
    *r = (uint8_t)p;
    *g = (uint8_t)v;
    *b = (uint8_t)t;
    break;
  case 3:
    *r = (uint8_t)p;
    *g = (uint8_t)q;
    *b = (uint8_t)v;
    break;
  case 4:
    *r = (uint8_t)t;
    *g = (uint8_t)p;
    *b = (uint8_t)v;
    break;
  case 5:
    *r = (uint8_t)v;
    *g = (uint8_t)p;
    *b = (uint8_t)q;
    break;
  }
}

uint8_t gos_color_hue(double p, double q, double t) {
  if (t < 0.0) t += 1.0;
  if (t > 1.0) t -= 1.0;
  if (t < (1.0 / 6.0)) {
    return GOS_COLOR_DENORMALIZE(p + (q - p) * 6.0 * t);
  }
  if (t < (1.0 / 2.0)) {
    return GOS_COLOR_DENORMALIZE(q);
  }
  if (t < (2.0 / 3.0)) {
    return GOS_COLOR_DENORMALIZE(p + (q - p) * (2.0 / 3.0 - t) * 6.0);
  }
  return GOS_COLOR_DENORMALIZE(p);
}
