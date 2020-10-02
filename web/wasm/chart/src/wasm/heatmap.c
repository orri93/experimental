#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#include <gos/data.h>
#include <gos/draw.h>
#include <gos/chart.h>
#include <gos/color.h>
#include <gos/development.h>
#include <gos/experimental.h>

#define GOS_HEATMAP_DEMO_WIDTH 512
#define GOS_HEATMAP_DEMO_HEIGHT 256

static void gos_heatmap_create_random_vector(gos_vector* vector, double f);
static bool gos_heatmap_demo(int width, int height);

#ifdef GOS_CHART_SDL_MAIN
int SDL_main(int argc, char** argv) {
  return gos_heatmap_demo(GOS_HEATMAP_DEMO_WIDTH, GOS_HEATMAP_DEMO_HEIGHT) ?
    EXIT_SUCCESS : EXIT_FAILURE;
}
#else
int main(int argc, char** argv) {
  return gos_heatmap_demo(GOS_HEATMAP_DEMO_WIDTH, GOS_HEATMAP_DEMO_HEIGHT) ?
    EXIT_SUCCESS : EXIT_FAILURE;
}
#endif

void gos_heatmap_create_random_vector(gos_vector* vector, double f) {
  int i;
  for (i = 0; i < vector->count; i++) {
    vector->data[i] = f * (((double)rand()) / RAND_MAX);
  }
}

bool gos_heatmap_demo(int width, int height) {
  int i;
  gos_matrix matrix;
  gos_vector vd1, vd2;
  gos_rgb_gradient gradient;
  bool demoresult = false;

  SDL_Surface* surface;

  if (gos_data_initialize_matrix(&matrix, width, height)) {
    if (gos_data_initialize_vector(&vd1, height)) {
      if (gos_data_initialize_vector(&vd2, width)) {
        if (gos_chart_create_gradient(&gradient)) {

          for (i = 0; i < width; i++) {
            gos_heatmap_create_random_vector(&vd1, 1.0);
            gos_data_set_vector_d1(&matrix, &vd1, i);
          }

          if (gos_draw_initialize(&surface, width, height)) {

            gos_draw_lock(surface);
            gos_draw_matrix(surface, &matrix, &gradient);
            gos_draw_unlock(surface);
            SDL_Flip(surface);

            for (i = 0; i < 128; i++) {
              gos_heatmap_create_random_vector(&vd1, 0.5);
              gos_data_shift_matrix_d1d1(&matrix);
              gos_data_set_vector_d1(&matrix, &vd1, matrix.countd1 - 2);
              gos_draw_lock(surface);
              gos_draw_matrix(surface, &matrix, &gradient);
              gos_draw_unlock(surface);
              SDL_Flip(surface);
            }

            for (i = 0; i < 64; i++) {
              gos_heatmap_create_random_vector(&vd1, 0.5);
              gos_data_shift_matrix_d2d1(&matrix);
              gos_data_set_vector_d1(&matrix, &vd1, 0);
              gos_draw_lock(surface);
              gos_draw_matrix(surface, &matrix, &gradient);
              gos_draw_unlock(surface);
              SDL_Flip(surface);
            }

            for (i = 0; i < 128; i++) {
              gos_heatmap_create_random_vector(&vd2, 0.5);
              gos_data_shift_matrix_d1d2(&matrix);
              gos_data_set_vector_d2(&matrix, &vd2, 0);
              gos_draw_lock(surface);
              gos_draw_matrix(surface, &matrix, &gradient);
              gos_draw_unlock(surface);
              SDL_Flip(surface);
            }

            for (i = 0; i < 64; i++) {
              gos_heatmap_create_random_vector(&vd2, 0.5);
              gos_data_shift_matrix_d2d2(&matrix);
              gos_data_set_vector_d2(&matrix, &vd2, height - 1);
              gos_draw_lock(surface);
              gos_draw_matrix(surface, &matrix, &gradient);
              gos_draw_unlock(surface);
              SDL_Flip(surface);
            }

            demoresult = true;
          }
          gos_draw_shutdown();

          gos_color_free_rgb_gradient(&gradient);
        }
        gos_data_free_vector(&vd2);
      }
      gos_data_free_vector(&vd1);
    }
    gos_data_free_matrix(&matrix);
  }

  return demoresult;
}
