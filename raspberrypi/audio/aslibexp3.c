/*
 * Raspberry Pi Audio output experiment 3 using C asound library
 *
 * Minimal code to produce a sound using ALSA library
 *   https://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_min_8c-example.html
 *
 * Compile:
 *   gcc -Wall -pthread -o aslibexp3 aslibexp3.c -lasound
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
  snd_pcm_sframes_t frames;

  unsigned char buffer[16*1024];

  for (i = 0; i < sizeof(buffer); i++) {
    buffer[i] = random() & 0xff;
  }

  /* Open the ALSA sound device */
  if ((err = snd_pcm_open(&pcm, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    fprintf(stderr, "cannot open audio device %s (%s)\n", PCM_DEVICE, snd_strerror(err));
    exit(EXIT_FAILURE);
  }

  if ((err = snd_pcm_set_params(pcm,
    SND_PCM_FORMAT_U8,
    SND_PCM_ACCESS_RW_INTERLEAVED,
    1,                /* channels         */
    48000,            /* sample rate      */
    1,                /* allow resampling */
    500000)) < 0) {   /* latency: 0.5 sec */
    fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < 16; i++) {
    frames = snd_pcm_writei(pcm, buffer, sizeof(buffer));
    if (frames < 0) {
      frames = snd_pcm_recover(pcm, frames, 0);
    }
    if (frames < 0) {
      fprintf(stderr, "snd_pcm_writei failed: %s\n", snd_strerror(err));
      break;
    }
    if (frames > 0 && frames < (long)sizeof(buffer)) {
      fprintf(stderr, "short write (expected %li, wrote %li)\n", (long)sizeof(buffer), frames);
    }
  }

  /* pass the remaining samples, otherwise they're dropped in close */
  err = snd_pcm_drain(pcm);
  if (err < 0) {
    fprintf(stderr, "snd_pcm_drain failed: %s\n", snd_strerror(err));
  }

  return EXIT_SUCCESS;
}
