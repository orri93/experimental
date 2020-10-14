#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include <gos/types.h>
#include <gos/noise.h>
#include <gos/signal.h>
#include <gos/geometry.h>
#include <gos/interpolate.h>

#include <tiera/tiera.h>

#define GOS_TIER_A_MESSAGE_SIZE 1024

static int _gos_tier_a_tool_count = 0;
static int _gos_tier_a_message_length = 0;
static char _gos_tier_a_message[GOS_TIER_A_MESSAGE_SIZE];
static gos_tier_a_d** _gos_tier_a_data_arr = NULL;

bool gos_tier_a_initialize(
  int toolcount,
  int datapointcount,
  int majorcount) {

  int i;
  gos_range_1d depth;
  gos_tier_a_d** datastat;
  gos_generation_1d datageneration, depthgeneration;

  size_t datapointsize;

  bool result;

  double datastart = 1.5;
  double datastep = 8.0;
  double datavar = 1.0;

  double depthspace = 100.0;

  depth.from = 50.0;
  depth.to = 100.0;

  datageneration.noise = 0.01;
  depthgeneration.noise = 0.01;

  datapointsize = datapointcount * sizeof(double);

  if (majorcount < datapointcount) {
    if (_gos_tier_a_data_arr == NULL) {
      _gos_tier_a_data_arr = (gos_tier_a_d**)calloc(
        toolcount, sizeof(gos_tier_a_d*));
      if (_gos_tier_a_data_arr != NULL) {
        datastat = _gos_tier_a_data_arr;
        _gos_tier_a_tool_count = toolcount;
        for (i = 0; i < toolcount; i++) {

          datageneration.range.from = datastart - datavar + datastep * i;
          datageneration.range.to = datastart + datavar + datastep * i;

          depthgeneration.range.from = depth.from + depthspace * i;
          depthgeneration.range.to = depth.to + depthspace * i;

          *datastat = (gos_tier_a_d*)malloc(
            sizeof(gos_tier_a_d));
          if ((*datastat) != NULL) {
            (*datastat)->count = 0;
            (*datastat)->data = (*datastat)->depth = NULL;
            (*datastat)->data = (double*)malloc(datapointsize);
            if ((*datastat)->data != NULL) {
              (*datastat)->depth = (double*)malloc(datapointsize);
              if ((*datastat)->depth != NULL) {
                (*datastat)->count = datapointcount;
                result = gos_tier_a_generate(
                  (*datastat),
                  &datageneration,
                  &depthgeneration,
                  majorcount);
                if (result) {
                  datastat++;
                } else {
                  gos_tier_a_shutdown();
                  _gos_tier_a_message_length = snprintf(
                    _gos_tier_a_message,
                    GOS_TIER_A_MESSAGE_SIZE,
                    "Failed to generate data for tool no. %d",
                    i);
                  return false;
                }
              } else {
                gos_tier_a_shutdown();
                _gos_tier_a_message_length = snprintf(
                  _gos_tier_a_message,
                  GOS_TIER_A_MESSAGE_SIZE,
                  "Out of memory while creating depth data points");
                return false;
              }
            } else {
              gos_tier_a_shutdown();
              _gos_tier_a_message_length = snprintf(
                _gos_tier_a_message,
                GOS_TIER_A_MESSAGE_SIZE,
                "Out of memory while creating data points");
              return false;
            }
          } else {
            gos_tier_a_shutdown();
            _gos_tier_a_message_length = snprintf(
              _gos_tier_a_message,
              GOS_TIER_A_MESSAGE_SIZE,
              "Out of memory while creating simulated data structure");
            return false;
          }
        }
      } else {
        gos_tier_a_shutdown();
        _gos_tier_a_message_length = snprintf(
          _gos_tier_a_message,
          GOS_TIER_A_MESSAGE_SIZE,
          "Out of memory while creating simulated data array");
        return false;
      }
    } else {
      gos_tier_a_shutdown();
      _gos_tier_a_message_length = snprintf(
        _gos_tier_a_message,
        GOS_TIER_A_MESSAGE_SIZE,
        "Simulated data has already been initialized");
      return false;
    }
  } else {
    gos_tier_a_shutdown();
    _gos_tier_a_message_length = snprintf(
      _gos_tier_a_message,
      GOS_TIER_A_MESSAGE_SIZE,
      "Major count is grater than data point count");
    return false;
  }
  return true;
}

bool gos_tier_a_generate(
  gos_tier_a_d* data,
  gos_generation_1d* datageneration,
  gos_generation_1d* depthgeneration,
  int major) {

  int div, i, j, n, e;

  double dat, dep;
  double depth, depthdistance, depthstep;
  double y0, y1, y2, y3;
  double mu, ms;

  double* v;
  double* vat;

  assert(data != NULL);
  assert(major < data->count);

  data->data_range.from = DBL_MAX;
  data->data_range.to = -DBL_MAX;
  data->depth_range.from = DBL_MAX;
  data->depth_range.to = -DBL_MAX;

  depth = depthgeneration->range.from;
  depthdistance = gos_geometry_distance_1d(&(depthgeneration->range));
  depthstep = depthdistance / (double)(data->count);

  e = data->count % major == 0 ? major + 3 : major + 4;
  v = vat = (double*)malloc(e * sizeof(double));

  if (v != NULL) {
    gos_signal_generate_range_random_y(v, e,
      datageneration->range.from,
      datageneration->range.to);

    div = data->count / major;
    ms = 1.0 / div;

    n = 0;
    for (i = 2; i < e - 1 && n < data->count; i++) {
      mu = 0.0;
      y0 = v[i - 2];
      y1 = v[i - 1];
      y2 = v[i];
      y3 = v[i + 1];

      for (j = 0; j < div && n < data->count; j++) {
        assert(n < data->count);
        assert(mu < 1.0);
        dat = gos_interpolate_cubic_catmull_rom(y0, y1, y2, y3, mu) +
          datageneration->noise *
          gos_noise_white(GOS_NOISE_DEFAULT_SEED, i, j);
        if(dat < data->data_range.from) {
          data->data_range.from = dat;
        }
        if(dat > data->data_range.to) {
          data->data_range.to = dat;
        } 
        data->data[n] += dat;

        dep = depth + depthgeneration->noise *
          gos_noise_white(GOS_NOISE_DEFAULT_SEED, i, j);
        if(dep < data->depth_range.from) {
          data->depth_range.from = dep;
        }
        if(dep > data->depth_range.to) {
          data->depth_range.to = dep;
        }
        data->depth[n] = dep;
        depth += depthstep;
        mu += ms;
        n++;
      }
    }
    assert(n == data->count);

    free(v);

    return true;
  } else {
    return false;
  }
}

gos_tier_a_d* gos_tier_a_get(int tool) {
  if (tool >= 0 && tool < _gos_tier_a_tool_count) {
    if (_gos_tier_a_data_arr != NULL) {
      if (_gos_tier_a_data_arr[tool] != NULL) {
        return _gos_tier_a_data_arr[tool];
      } else {
        _gos_tier_a_message_length = snprintf(
          _gos_tier_a_message,
          GOS_TIER_A_MESSAGE_SIZE,
          "The data for the requested tool is undefined");
        return NULL;
      }
    } else {
      _gos_tier_a_message_length = snprintf(
        _gos_tier_a_message,
        GOS_TIER_A_MESSAGE_SIZE,
        "The data array is undefined");
      return NULL;
    }
  } else {
    _gos_tier_a_message_length = snprintf(
      _gos_tier_a_message,
      GOS_TIER_A_MESSAGE_SIZE,
      "The data tool number is out of range");
    return NULL;
  }
}

bool gos_tier_a_get_data(double* value, int tool, int index) {
  gos_tier_a_d* dst = gos_tier_a_get(tool);
  if (dst != NULL) {
    *value = dst->data[index];
    return true;
  } else {
    return false;
  }
}

bool gos_tier_a_get_depth(double* value, int tool, int index) {
  gos_tier_a_d* dst = gos_tier_a_get(tool);
  if (dst != NULL) {
    *value = dst->depth[index];
    return true;
  } else {
    return false;
  }
}

bool gos_tier_a_get_data_range(double* from, double* to) {
  int i;
  gos_tier_a_d* tp;
  *from = DBL_MAX;
  *to = -DBL_MAX;
  if(_gos_tier_a_data_arr != NULL){
    for(i = 0; i < _gos_tier_a_tool_count; i++) {
      tp = gos_tier_a_get(i);
      if(tp != NULL) {
        if(tp->data_range.from < *from) {
          *from = tp->data_range.from;
        }
        if(tp->data_range.to > *to) {
          *to = tp->data_range.to;
        }
      } else {
        return false;
      }
    }
    return true;
  } else {
    _gos_tier_a_message_length = snprintf(
      _gos_tier_a_message,
      GOS_TIER_A_MESSAGE_SIZE,
      "The data array is undefined");
    return false;
  }
}

bool gos_tier_a_get_depth_range(double* from, double* to) {
  int i;
  gos_tier_a_d* tp;
  *from = DBL_MAX;
  *to = -DBL_MAX;
  if(_gos_tier_a_data_arr != NULL){
    for(i = 0; i < _gos_tier_a_tool_count; i++) {
      tp = gos_tier_a_get(i);
      if(tp != NULL) {
        if(tp->data_range.from < *from) {
          *from = tp->data_range.from;
        }
        if(tp->data_range.to > *to) {
          *to = tp->data_range.to;
        }
      } else {
        return false;
      }
    }
    return true;
  } else {
    _gos_tier_a_message_length = snprintf(
      _gos_tier_a_message,
      GOS_TIER_A_MESSAGE_SIZE,
      "The data array is undefined");
    return false;
  }
} 

const char* gos_tier_a_message() {
  return (const char*)_gos_tier_a_message;
}

const char* gos_tier_a_message_length(int* length) {
  *length = _gos_tier_a_message_length;
  return gos_tier_a_message();
}

void gos_tier_a_shutdown() {
  int i;
  gos_tier_a_d* datastr;
  if (_gos_tier_a_data_arr != NULL) {
    for (i = 0; i < _gos_tier_a_tool_count; i++) {
      if (_gos_tier_a_data_arr[i] != NULL) {
        datastr = _gos_tier_a_data_arr[i];
        if (datastr->data != NULL) {
          free(datastr->data);
          datastr->data = NULL;
        }
        if (datastr->depth != NULL) {
          free(datastr->depth);
          datastr->depth = NULL;
        }
        free(_gos_tier_a_data_arr[i]);
        _gos_tier_a_data_arr[i] = NULL;
      }
    }
    free(_gos_tier_a_data_arr);
    _gos_tier_a_data_arr = NULL;
    _gos_tier_a_tool_count = 0;
  }
}
