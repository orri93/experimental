#include <modules/mc/mandel.h>

#define GOS_EXPB_MODULES_MC_MANDEL_ZERO_F 0.0L
#define GOS_EXPB_MODULES_MC_MANDEL_ZERO_IT 0

gosmf gos_mandel_ratio(gosmf f, gosmf t, gosmf r) {
  return f + r * (t - f);
}

gosmf gos_mandel_map(gosmf f, gosmf t, int p, int m) {
  return gos_mandel_ratio(f, t, (gosmf)p / (gosmf)m);
}

gosmf gos_mandel_map_range(gosmrange* range, int p, int m) {
  return gos_mandel_map(range->f, range->t, p, m);
}

#if defined(GOS_EXPB_MODULES_MC_MANDEL_OPTIMIZED)
gosmit gos_mandel(gosmf x0, gosmf y0, gosmf maxf, gosmit maxi) {
  gosmit i;
  gosmf x, y, x2, y2;
  x = GOS_EXPB_MODULES_MC_MANDEL_ZERO_F;
  y = GOS_EXPB_MODULES_MC_MANDEL_ZERO_F;
  x2 = GOS_EXPB_MODULES_MC_MANDEL_ZERO_F;
  y2 = GOS_EXPB_MODULES_MC_MANDEL_ZERO_F;
  i = GOS_EXPB_MODULES_MC_MANDEL_ZERO_IT;
  while (x2 + y2 <= maxf && i < maxi) {
    y = 2 * x * y + y0;
    x = x2 - y2 + x0;
    x2 = x * x;
    y2 = y * y;
    i++;
  }
  return i;
}
#else
gosmit gos_mandel(gosmf x0, gosmf y0, gosmf maxf, gosmit maxi) {
  gosmit i;
  gosmf x, y, t;
  x = GOS_EXPB_MODULES_MC_MANDEL_ZERO_F;
  y = GOS_EXPB_MODULES_MC_MANDEL_ZERO_F;
  i = GOS_EXPB_MODULES_MC_MANDEL_ZERO_IT;
  while (x * x + y * y <= maxf && i < maxi) {
    t = x * x - y * y + x0;
    y = 2 * x * y + y0;
    x = t;
    i++;
  }
  return i;
}
#endif
