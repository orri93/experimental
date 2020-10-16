#include <wasm/chart.h>

#define GOS_CHART_COLOR_STOP_COUNT 6

int gos_chart_vector_index(gos_point_2d_vector* vector, int row) {
  int i;
  double r = row;
  for (i = 0; i < vector->count - 1; i++) {
    if (r >= (vector->points[i]).x && r <= (vector->points[i + 1]).x) {
      return i;
    }
  }
  return -1;
}

bool gos_chart_create_gradient(gos_rgb_gradient* gradient) {
  int i;
  int* gsizeat;
  int gradientsize[GOS_CHART_COLOR_STOP_COUNT - 1];
  gos_rgb stops[GOS_CHART_COLOR_STOP_COUNT];
  gos_rgb* rgbat = stops;

  gos_color_assign_rgb32(rgbat++, 0x07049b);  /* Dark blue */
  gos_color_assign_rgb32(rgbat++, 0x02f7f3);  /* Cyan */
  gos_color_assign_rgb32(rgbat++, 0x09f725);  /* Green */
  gos_color_assign_rgb32(rgbat++, 0xf4ec04);  /* Yellow */
  gos_color_assign_rgb32(rgbat++, 0xf79d01);  /* Orange */
  gos_color_assign_rgb32(rgbat++, 0x8c0101);  /* Dark red */

  /* Equal sized gradient */
  gsizeat = gradientsize;
  for (i = 0; i < GOS_CHART_COLOR_STOP_COUNT - 1; i++) {
    *(gsizeat++) = 64;
  }

  return gos_color_create_rgb_gradient(
    gradient,
    stops,
    gradientsize,
    GOS_CHART_COLOR_STOP_COUNT,
    GOS_COLOR_GAMMA);
}
