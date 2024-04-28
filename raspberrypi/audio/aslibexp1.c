/*
 * Raspberry Pi Audio output experiment 1 using C asound library
 *
 * Compile:
 *   gcc -Wall -pthread -o aslibexp1 aslibexp1.c -lm -lasound
 *
 * Usage:
 *   ./aslibexp1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <alsa/asoundlib.h>

#define PCM_DEVICE "default"

int main(int argc, char *argv[])
{
  int i;
  snd_pcm_t *pcm;

  /* Open the ALSA sounde device */
  snd_pcm_open(&pcm, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);

  /* Set the ALSA sound device parameters */
  snd_pcm_set_params(pcm,
    SND_PCM_FORMAT_S16_LE,
    SND_PCM_ACCESS_RW_INTERLEAVED,
    2, /* channels */
    44100, /* sample rate */
    1, /* allow resampling */
    0); /* latency */

  /* Generate a sine wave */
  for (i = 0; i < 44100; i++) {
    short sample[2];
    sample[0] = (short) (32767.0 * sin(2 * M_PI * 440 * i / 44100));
    sample[1] = sample[0];
    snd_pcm_writei(pcm, sample, 2);
  }

  /* Close the ALSA sound device */
  snd_pcm_close(pcm);

  return EXIT_SUCCESS;
}
