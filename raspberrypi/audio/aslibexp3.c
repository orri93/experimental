/*
 * Raspberry Pi Audio output experiment 3 using C asound library
 *
 * Compile:
 *   gcc -Wall -pthread -o aslibexp3 aslibexp3.c -lm -lasound
 *
 * Usage:
 *   ./aslibexp3
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <alsa/asoundlib.h>

#define PCM_DEVICE "default"

int main(int argc, char *argv[]) {
  int err;
  unsigned int i;
  snd_pcm_t *pcm;

  unsigned char buffer[16*1024];
}
