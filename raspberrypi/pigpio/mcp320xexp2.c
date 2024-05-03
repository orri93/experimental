/*
 * Raspberry Pi MCP3204/3208 experiment 2 using C PiGPIO library
 *
 * Compile:
 *   gcc -Wall -pthread -o mcp320xexp2 mcp320xexp2.c -lpigpio
 *
 * Usage:
 *   ./mcp320xexp2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <pigpio.h>

#include "mcp320x.h"
#include "statistic.h"

#define SPI_CHANNEL       0
#define SPI_SPEED   1000000

typedef struct {
  uint16_t channel;
  uint16_t value;
  Statistic statistic;
} ChStVal;

int main(int argc, char *argv[]) {
  int i, j;
  uint16_t value;
  double mean, min, max, sd;
  Statistic *statistic;
  ChStVal chstval[8];
  MCP320X mcp320x;
  Mcp320xSpi spi;

  chstval[0].channel = MCP3208_SINGLE_0;
  chstval[1].channel = MCP3208_SINGLE_1;
  chstval[2].channel = MCP3208_SINGLE_2;
  chstval[3].channel = MCP3208_SINGLE_3;
  chstval[4].channel = MCP3208_SINGLE_4;
  chstval[5].channel = MCP3208_SINGLE_5;
  chstval[6].channel = MCP3208_SINGLE_6;
  chstval[7].channel = MCP3208_SINGLE_7;

  for (i = 0; i < 8; i++) {
    statistic_init(&(chstval[i].statistic));
  }

  if (gpioInitialise() < 0) {
    fprintf(stderr, "Failed to initialize GPIO\n");
    return 1;
  }

  /* Configure the SPI */
  spi.channel = SPI_CHANNEL;
  spi.baud = SPI_SPEED;
  spi.flags = 0;

  mcp320x_init(&mcp320x, &spi, MCP320X_MCP3208);

  for (i = 0; i < 100; i++) {
    for (j = 0; j < 8; j++) {
      value = mcp320x_read(&mcp320x, chstval[j].channel);
      statistic_add(&(chstval[j].statistic), (double)value);
    }
  }

  printf("Channel\tMin\tMax\tMean\tSD\n");
  for (i = 0; i < 8; i++) {
    statistic = &(chstval[i].statistic);
    mean = statistic_mean(statistic);
    sd = statistic_sd(statistic);
    printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\n", i, statistic->min, statistic->max, mean, sd);
  }

  for (i = 0; i < 8; i++) {
    statistic_free(&(chstval[i].statistic));
  }

  gpioTerminate();

  return 0;
}
