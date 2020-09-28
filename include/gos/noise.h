#ifndef GOS_NOISE_H_
#define GOS_NOISE_H_

#define GOS_NOISE_DEFAULT_SEED 1337

#ifdef __cplusplus
extern "C" {
#endif

int gos_noise_random_maximum_integer(int maximum);

double gos_noise_white(int seed, int x, int y);

#ifdef __cplusplus
}
#endif

#endif
