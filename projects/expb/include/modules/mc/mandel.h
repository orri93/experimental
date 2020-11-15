#ifndef GOS_EXPB_MODULES_MC_MANDEL_H_
#define GOS_EXPB_MODULES_MC_MANDEL_H_

#include <stdbool.h>

typedef int gosmit;

typedef long double gosmf;

typedef struct gosmpoint {
  gosmf x;
  gosmf y;
} gosmpoint;

typedef struct gosmrange {
  gosmf f;
  gosmf t;
} gosmrange;

typedef struct gosmrect {
  gosmrange a;
  gosmrange b;
} gosmrect;

#ifdef __cplusplus
extern "C" {
#endif

gosmf gos_mandel_ratio(gosmf f, gosmf t, gosmf r);
gosmf gos_mandel_map(gosmf f, gosmf t, int p, int m);
gosmf gos_mandel_map_range(gosmrange* range, int p, int m);

/*
 * Mandel 
 *
 *  x0: Scaled x coordinate of pixel
 *      Scaled to lie in the Mandelbrot X scale (-2.5, 1))
 *  y0: Scaled y coordinate of pixel
 *      Scaled to lie in the Mandelbrot Y scale (-1, 1))
 */
gosmit gos_mandel(gosmf x0, gosmf y0, gosmf maxf, gosmit maxi);

#ifdef __cplusplus
}
#endif

#endif
