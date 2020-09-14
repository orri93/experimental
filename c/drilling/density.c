#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <gos/types.h>
#include <gos/noise.h>
#include <gos/signal.h>
#include <gos/geometry.h>
#include <gos/interpolate.h>
#include <gos/drilling/density.h>

#define GOS_EXP_DRILLING_MESSAGE_SIZE 1024

static int _gos_exp_drilling_density_tool_count = 0;
static gos_drilling_density** _gos_exp_drilling_density_ecd = NULL;
static int _gos_exp_drilling_message_length = 0;
static char _gos_exp_drilling_message[GOS_EXP_DRILLING_MESSAGE_SIZE];

bool gos_exp_drilling_density_initialize(
  int toolcount,
  int datapointcount,
  int majorcount) {

  int i;
  gos_range_1d md;
  gos_drilling_density* ecd;
  gos_drilling_density** ecdat;
  gos_generation_1d ecdgeneration, mdgeneration;

  size_t datapointsize;

  bool result;

  double ecdstart = 1.5;
  double ecdstep = 8.0;
  double ecdvar = 1.0;

  md.from = 300.0;
  md.to = 900.0;

  ecdgeneration.noise = 0.01;
  mdgeneration.noise = 0.01;

  datapointsize = datapointcount * sizeof(double);

  if (majorcount < datapointcount) {
    if (_gos_exp_drilling_density_ecd == NULL) {
      _gos_exp_drilling_density_ecd = (gos_drilling_density**)calloc(
        toolcount, sizeof(gos_drilling_density*));
      if (_gos_exp_drilling_density_ecd != NULL) {
        ecdat = _gos_exp_drilling_density_ecd;
        _gos_exp_drilling_density_tool_count = toolcount;
        for (i = 0; i < toolcount; i++) {

          ecdgeneration.range.from = ecdstart - ecdvar + ecdstep * i;
          ecdgeneration.range.to = ecdstart + ecdvar + ecdstep * i;

          mdgeneration.range.from = md.from + 32.0 * i;
          mdgeneration.range.to = md.to + 32.0 * i;

          *ecdat = (gos_drilling_density*)malloc(
            sizeof(gos_drilling_density));
          if ((*ecdat) != NULL) {
            (*ecdat)->count = 0;
            (*ecdat)->density = (*ecdat)->depth = NULL;
            (*ecdat)->density = (double*)malloc(datapointsize);
            if ((*ecdat)->density != NULL) {
              (*ecdat)->depth = (double*)malloc(datapointsize);
              if ((*ecdat)->depth != NULL) {
                (*ecdat)->count = datapointcount;
                result = gos_exp_drilling_density_generate(
                  (*ecdat),
                  &ecdgeneration,
                  &mdgeneration,
                  majorcount);
                if (result) {
                  ecdat++;
                } else {
                  gos_exp_drilling_density_shutdown();
                  _gos_exp_drilling_message_length = snprintf(
                    _gos_exp_drilling_message,
                    GOS_EXP_DRILLING_MESSAGE_SIZE,
                    "Failed to generate ECD for tool no. %d",
                    i);
                  return false;
                }
              } else {
                gos_exp_drilling_density_shutdown();
                _gos_exp_drilling_message_length = snprintf(
                  _gos_exp_drilling_message,
                  GOS_EXP_DRILLING_MESSAGE_SIZE,
                  "Out of memory while creating depth data points");
                return false;
              }
            } else {
              gos_exp_drilling_density_shutdown();
              _gos_exp_drilling_message_length = snprintf(
                _gos_exp_drilling_message,
                GOS_EXP_DRILLING_MESSAGE_SIZE,
                "Out of memory while creating density data points");
              return false;
            }
          } else {
            gos_exp_drilling_density_shutdown();
            _gos_exp_drilling_message_length = snprintf(
              _gos_exp_drilling_message,
              GOS_EXP_DRILLING_MESSAGE_SIZE,
              "Out of memory while creating ECD simulated data structure");
            return false;
          }
        }
      } else {
        gos_exp_drilling_density_shutdown();
        _gos_exp_drilling_message_length = snprintf(
          _gos_exp_drilling_message,
          GOS_EXP_DRILLING_MESSAGE_SIZE,
          "Out of memory while creating drilling density simulated data array");
        return false;
      }
    } else {
      gos_exp_drilling_density_shutdown();
      _gos_exp_drilling_message_length = snprintf(
        _gos_exp_drilling_message,
        GOS_EXP_DRILLING_MESSAGE_SIZE,
        "Drilling density simulated data has already been initialized");
      return false;
    }
  } else {
    gos_exp_drilling_density_shutdown();
    _gos_exp_drilling_message_length = snprintf(
      _gos_exp_drilling_message,
      GOS_EXP_DRILLING_MESSAGE_SIZE,
      "Major count is grater than data point count");
    return false;
  }
  return true;
}

gos_drilling_density* gos_exp_drilling_density_ecd_get(int tool) {
  if (tool >= 0 && tool < _gos_exp_drilling_density_tool_count) {
    if (_gos_exp_drilling_density_ecd != NULL) {
      if (_gos_exp_drilling_density_ecd[tool] != NULL) {
        return _gos_exp_drilling_density_ecd[tool];
      } else {
        _gos_exp_drilling_message_length = snprintf(
          _gos_exp_drilling_message,
          GOS_EXP_DRILLING_MESSAGE_SIZE,
          "The ECD for the requested tool is undefined");
        return NULL;
      }
    } else {
      _gos_exp_drilling_message_length = snprintf(
        _gos_exp_drilling_message,
        GOS_EXP_DRILLING_MESSAGE_SIZE,
        "The ECD array is undefined");
      return NULL;
    }
  } else {
    _gos_exp_drilling_message_length = snprintf(
      _gos_exp_drilling_message,
      GOS_EXP_DRILLING_MESSAGE_SIZE,
      "The ECD tool number is out of range");
    return NULL;
  }
}


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

void gos_exp_drilling_density_shutdown() {
  int i;
  gos_drilling_density* ecd;
  if (_gos_exp_drilling_density_ecd != NULL) {
    for (i = 0; i < _gos_exp_drilling_density_tool_count; i++) {
      if (_gos_exp_drilling_density_ecd[i] != NULL) {
        ecd = _gos_exp_drilling_density_ecd[i];
        if (ecd->density != NULL) {
          free(ecd->density);
          ecd->density = NULL;
        }
        if (ecd->depth != NULL) {
          free(ecd->depth);
          ecd->depth = NULL;
        }
        free(_gos_exp_drilling_density_ecd[i]);
        _gos_exp_drilling_density_ecd[i] = NULL;
      }
    }
    free(_gos_exp_drilling_density_ecd);
    _gos_exp_drilling_density_ecd = NULL;
    _gos_exp_drilling_density_tool_count = 0;
  }
}

const char* gos_exp_drilling_message() {
  return (const char*)_gos_exp_drilling_message;
}

const char* gos_exp_drilling_message_length(int* length) {
  *length = _gos_exp_drilling_message_length;
  return gos_exp_drilling_message();
}
