#ifndef GOS_COLOR_H_
#define GOS_COLOR_H_

#include <stdlib.h>
#include <stdint.h>

#define GOS_COLOR_MAX(a,b,c) ( max( (a) , max( (b) , (c) )) )
#define GOS_COLOR_MIN(a,b,c) ( min( (a) , min( (b) , (c) )) )

#ifdef __cplusplus
extern "C" {
#endif

void gos_color_rgb_to_hsl(
  uint8_t r, uint8_t g, uint8_t b,
  double* h, double* s, double* l);

void gos_color_rgb_to_hsv(
  uint8_t r, uint8_t g, uint8_t b,
  double* h, double* s, double* v);

void gos_color_hsl_to_rgb(
  double h, double s, double l,
  uint8_t* r, uint8_t* g, uint8_t* b);

void gos_color_hsv_to_rgb(
  double h, double s, double v,
  uint8_t* r, uint8_t* g, uint8_t* b);

uint8_t gos_color_hue_to_rgb(double p, double q, double t);

#ifdef __cplusplus
}
#endif

#endif
