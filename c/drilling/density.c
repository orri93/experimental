#include <stdlib.h>
#include <assert.h>

#include <gos/types.h>
#include <gos/noise.h>
#include <gos/signal.h>
#include <gos/geometry.h>
#include <gos/interpolate.h>
#include <gos/drilling/density.h>

bool gos_exp_drilling_density_generate(
  gos_drilling_density* ecd,
  gos_generation_1d* densitygeneration,
  gos_generation_1d* depthgeneration,
  int major) {

  int div, i, j, n, e;

  double depth, depthdistance, depthstep;
  double y0, y1, y2, y3;
  double mu, ms;

  double* v;
  double* vat;

  assert(major < ecd->count);
  assert(ecd != NULL);

  depth = depthgeneration->range.from;
  depthdistance = gos_geometry_distance_1d(&(depthgeneration->range));
  depthstep = depthdistance / (double)(ecd->count);

  e = ecd->count % major == 0 ? major + 3 : major + 4;
  v = vat = (double*)malloc(e * sizeof(double));
  
  if (v != NULL) {
    gos_signal_generate_range_random_y(v, e,
      densitygeneration->range.from,
      densitygeneration->range.to);

    div = ecd->count / major;
    ms = 1.0 / div;

    n = 0;
    for (i = 2; i < e - 1 && n < ecd->count; i++) {
      mu = 0.0;
      y0 = v[i - 2];
      y1 = v[i - 1];
      y2 = v[i];
      y3 = v[i + 1];
      for (j = 0; j < div && n < ecd->count; j++) {
        assert(n < ecd->count);
        assert(mu < 1.0);
        ecd->density[n] = gos_interpolate_cubic_catmull_rom(y0, y1, y2, y3, mu);
        ecd->density[n] += densitygeneration->noise *
          gos_noise_white(GOS_NOISE_DEFAULT_SEED, i, j);
        ecd->depth[n] = depth;
        ecd->depth[n] += depthgeneration->noise *
          gos_noise_white(GOS_NOISE_DEFAULT_SEED, i, j);
        depth += depthstep;
        mu += ms;
        n++;
      }
    }
    assert(n == ecd->count);

    free(v);

    return true;
  } else {
    return false;
  }
}
