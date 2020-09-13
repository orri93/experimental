#ifndef GOS_PERLIN_H_
#define GOS_PERLIN_H_

/*
 * 
 * Perlin noise
 * 
 * Perlin noise is a type of gradient noise developed by Ken Perlin in 1983
 * as a result of his frustration with the "machine-like" look of
 * computer-generated imagery (CGI) at the time. He formally described his
 * findings in a SIGGRAPH paper in 1985 called An image Synthesizer. In 1997,
 * Perlin was awarded an Academy Award for Technical Achievement for
 * creating the algorithm
 * 
 * The following is a two-dimensional implementation of Classical Perlin Noise
 * 
 * See https://en.wikipedia.org/wiki/Perlin_noise
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GOS_PERLIN_FLOAT
float gos_perlin_f(float x, float y);
#else
#ifndef GOS_PERLIN_DOUBLE
#define GOS_PERLIN_DOUBLE
#endif
#endif

#ifdef GOS_PERLIN_DOUBLE
double gos_perlin(double x, double y);
#endif

#ifdef __cplusplus
}
#endif

#endif
