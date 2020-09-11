#include <assert.h>

#include <gos/color.h>

#define GOS_COLOR_MAX_RGB_F 255.0
#define GOS_COLOR_ROUND_RGB_F 255.5

void gos_color_interpolation_rgb_init(
  gos_color_interpolation* interpolation,
  gos_color* first,
  gos_color* second) {
  assert(interpolation != NULL);
  assert(first != NULL);
  assert(second != NULL);
}

void gos_color_rgb_to_hsl(
  uint8_t r, uint8_t g, uint8_t b,
  double* h, double* s, double* l) {
  double rd = ((double)r) / GOS_COLOR_MAX_RGB_F;
  double gd = ((double)g) / GOS_COLOR_MAX_RGB_F;
  double bd = ((double)b) / GOS_COLOR_MAX_RGB_F;
  double maximum = GOS_COLOR_MAX(rd, gd, bd);
  double minimum = GOS_COLOR_MIN(rd, gd, bd);
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
  double rd = (double)r / GOS_COLOR_MAX_RGB_F;
  double gd = (double)g / GOS_COLOR_MAX_RGB_F;
  double bd = (double)b / GOS_COLOR_MAX_RGB_F;
  double maximum = GOS_COLOR_MAX(rd, gd, bd);
  double minimum = GOS_COLOR_MIN(rd, gd, bd);
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
    *r = *g = *b = (uint8_t)(GOS_COLOR_ROUND_RGB_F * l); // achromatic
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
    return (uint8_t)
      (GOS_COLOR_ROUND_RGB_F * (p + (q - p) * 6.0 * t));
  }
  if (t < (1.0 / 2.0)) {
    return (uint8_t)
      (GOS_COLOR_ROUND_RGB_F * q);
  }
  if (t < (2.0 / 3.0)) {
    return (uint8_t)
      (GOS_COLOR_ROUND_RGB_F * (p + (q - p) * (2.0 / 3.0 - t) * 6.0));
  }
  return (uint8_t)
    (GOS_COLOR_ROUND_RGB_F * p);
}
