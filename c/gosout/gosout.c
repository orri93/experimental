#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <gos/types.h>
#include <gos/noise.h>
#include <gos/signal.h>
#include <gos/interpolate.h>
#include <gos/drilling/density.h>

#define GOS_SIGNAL_COUNT 12
#define GOS_DRILLING_TOOL_COUNT 3
#define GOS_DRILLING_MAJOR_COUNT 32
#define GOS_DRILLING_DATA_POINT_COUNT GOS_DRILLING_MAJOR_COUNT * 1024


static int first(int argc, char** argv);
static int second(int argc, char** argv);
static int third(int argc, char** argv);

int main(int argc, char** argv) {
  return third(argc, argv);
}

int first(int argc, char** argv) {
  const double delta = 0.1;
  const double noiseratio = 0.1;
  int i, j;

  double n, d, s;
  double x, x1, x2;
  double y, y0, y1, y2, y3;

  gos_point_2d v[GOS_SIGNAL_COUNT];
  gos_range_2d range;

  srand(1); /* Fixed random output */

  range.first.from = 1.0;
  range.first.to = 32.0;
  range.second.from = 2.0;
  range.second.to = 8.0;

  gos_signal_generate_2d_range_random_y(v, &range, GOS_SIGNAL_COUNT);

  for (i = 1; i < GOS_SIGNAL_COUNT - 2; i++) {
    y0 = v[i - 1].y;
    x1 = v[i].x;
    y1 = v[i].y;
    x2 = v[i + 1].x;
    y2 = v[i + 1].y;
    y3 = v[i + 2].y;
    x = x1;
    d = x2 - x1;
    s = delta * d;
    j = 0;
    for (n = 0.0; n < 1.0; n += delta) {
      y = gos_interpolate_cubic_catmull_rom(y0, y1, y2, y3, n);
      y += noiseratio * gos_noise_white(GOS_NOISE_DEFAULT_SEED, i, j++);
      x += s;
      printf("%f %f\n", x, y);
    }
  }

  return EXIT_SUCCESS;
}

int second(int argc, char** argv) {
  int mainres = EXIT_SUCCESS;
  bool result;
  int i, j;

  double ecdnoise = 0.0;
  double mdnoise = 0.0;

  double ecdstart = 1.5;
  double ecdstep = 8.0;
  double ecdvar = 1.0;

  double* density;
  double* depth;

  gos_drilling_density tool[GOS_DRILLING_TOOL_COUNT];
  gos_generation_1d ecdg[GOS_DRILLING_TOOL_COUNT];
  gos_generation_1d mdg[GOS_DRILLING_TOOL_COUNT];
  gos_range_1d md;

  md.from = 300.0;
  md.to = 900.0;

  srand(1); /* Fixed random output */

  if (argc > 1) {
    ecdnoise = atof(argv[1]);
    if (argc > 2) {
      mdnoise = atof(argv[2]);
    }
  }

  /* First round */
  for (i = 0; i < GOS_DRILLING_TOOL_COUNT; i++) {
    ecdg[i].range.from = ecdstart - ecdvar + ecdstep * i;
    ecdg[i].range.to = ecdstart + ecdvar + ecdstep * i;
    ecdg[i].noise = ecdnoise;
    mdg[i].range.from = md.from + 32.0 * i;
    mdg[i].range.to = md.to + 32.0 * i;
    mdg[i].noise = mdnoise;
    tool[i].count = GOS_DRILLING_DATA_POINT_COUNT;
    tool[i].density = NULL;
    tool[i].depth = NULL;
  }

  /* Allocate memory and generate */
  for (i = 0; i < GOS_DRILLING_TOOL_COUNT; i++) {
    tool[i].density = (double*)malloc(tool[i].count * sizeof(double));
    if (tool[i].density != NULL) {
      tool[i].depth = (double*)malloc(tool[i].count * sizeof(double));
      if (tool[i].depth != NULL) {
        result = gos_exp_drilling_density_generate(
          &(tool[i]),
          &(ecdg[i]),
          &(mdg[i]),
          GOS_DRILLING_MAJOR_COUNT);
        if (!result) {
          fprintf(
            stderr,
            "Failed to generate ECD for tool no. %d",
            i);
          mainres = EXIT_FAILURE;
          goto exit_second_function;
        }
      } else {
        fprintf(
          stderr,
          "Failed to allocate memory for depth for tool no. %d",
          i);
        mainres = EXIT_FAILURE;
        goto exit_second_function;
      }
    } else {
      fprintf(
        stderr,
        "Failed to allocate memory for density for tool no. %d",
        i);
      mainres = EXIT_FAILURE;
      goto exit_second_function;
    }
  }

  /* Output header */
  printf("i");
  for (i = 0; i < GOS_DRILLING_TOOL_COUNT; i++) {
    printf(" md%d density%d", i+1, i+1);
  }
  printf("\n");

  /* Output data */
  for (i = 0; i < GOS_DRILLING_DATA_POINT_COUNT; i++) {
    printf("%d", i);
    for (j = 0; j < GOS_DRILLING_TOOL_COUNT; j++) {
      density = tool[j].density;
      depth = tool[j].depth;
      printf(" %f %f", depth[i], density[i]);
    }
    printf("\n");
  }

exit_second_function:
  for (i = 0; i < GOS_DRILLING_TOOL_COUNT; i++) {
    if (tool[i].density != NULL) {
      free(tool[i].density);
      tool[i].density = NULL;
    }
    if (tool[i].depth != NULL) {
      free(tool[i].depth);
      tool[i].depth = NULL;
    }
  }

  return mainres;
}

int third(int argc, char** argv) {
  int i, j, c, f, t, mainres = EXIT_SUCCESS;

  gos_drilling_density* ecd;

  double* density;
  double* depth;

  bool result;

  if (argc > 1) {

    c = atoi(argv[1]);

    switch (c) {
    case 1:
      f = 0;
      t = GOS_DRILLING_DATA_POINT_COUNT / 2;
      break;
    case 2:
      f = GOS_DRILLING_DATA_POINT_COUNT / 2;
      t = GOS_DRILLING_DATA_POINT_COUNT;
      break;
    default:
      fprintf(
        stderr,
        "%d is not acceptable command. Acceptable command is 1 or 2.",
        c);
      return EXIT_FAILURE;
    }

    result = gos_exp_drilling_density_initialize(
      GOS_DRILLING_TOOL_COUNT,
      GOS_DRILLING_DATA_POINT_COUNT,
      GOS_DRILLING_MAJOR_COUNT);

    /* JSON header */
    printf("{\n");

    for (i = 0; i < GOS_DRILLING_TOOL_COUNT; i++) {
      printf("  \"tool%d\": [\n", i);
    }

    if (result) {
      /* Output data */
      for (i = 0; i < GOS_DRILLING_DATA_POINT_COUNT; i++) {
        printf("%d", i);
        for (j = 0; j < GOS_DRILLING_TOOL_COUNT; j++) {
          ecd = gos_exp_drilling_density_ecd_get(j);
          density = ecd->density;
          depth = ecd->depth;
          printf(" %f %f", depth[i], density[i]);
        }
        printf("\n");
      }
    } else {
      fprintf(
        stderr,
        "Failed to initialize drilling density: %s",
        gos_exp_drilling_message());
      mainres = EXIT_FAILURE;
    }

    gos_exp_drilling_density_shutdown();

  } else {

    fprintf(
      stderr,
      "Command is missing. Acceptable command is 1 or 2.");

    return EXIT_FAILURE;

  }

  return mainres;
}