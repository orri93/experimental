/*
 * Raspberry Pi MCP3204/3208 experiment 1 using C PiGPIO library
 *
 * Compile:
 *   gcc -Wall -pthread -o mcp320xexp1 mcp320xexp1.c -lpigpio
 *
 * Usage:
 *   ./mcp320xexp1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <pigpio.h>

#include "mcp320x.h"

#define SPI_CHANNEL       0
#define SPI_SPEED   1000000

static char* binray_repr(uint16_t value);

int main(int argc, char *argv[]) {
  uint16_t value;
  MCP320X mcp320x;
  Mcp320xSpi spi;

  if (gpioInitialise() < 0) {
    fprintf(stderr, "Failed to initialize GPIO\n");
    return 1;
  }

  /* Configure the SPI */
  spi.channel = SPI_CHANNEL;
  spi.baud = SPI_SPEED;
  spi.flags = 0;

  mcp320x_init(&mcp320x, &spi, MCP320X_MCP3208, 3300);

  value = mcp320x_read(&mcp320x, MCP3208_SINGLE_0);
  printf("Channel 0: %d\n", value);
  value = mcp320x_read(&mcp320x, MCP3208_SINGLE_1);
  printf("Channel 1: %d\n", value);
  value = mcp320x_read(&mcp320x, MCP3208_SINGLE_2);
  printf("Channel 2: %d\n", value);
  value = mcp320x_read(&mcp320x, MCP3208_SINGLE_3);
  printf("Channel 3: %d\n", value);
  value = mcp320x_read(&mcp320x, MCP3208_SINGLE_4);
  printf("Channel 4: %d\n", value);
  value = mcp320x_read(&mcp320x, MCP3208_SINGLE_5);
  printf("Channel 5: %d\n", value);
  value = mcp320x_read(&mcp320x, MCP3208_SINGLE_6);
  printf("Channel 6: %d\n", value);
  value = mcp320x_read(&mcp320x, MCP3208_SINGLE_7);
  printf("Channel 7: %d\n", value);

  gpioTerminate();

  return 0;
}

char* binray_repr(uint16_t value) {
  static char bin_str[17];
  bin_str[16] = '\0';
  for (int i = 15; i >= 0; i--) {
    bin_str[15 - i] = (value & (1 << i)) ? '1' : '0';
  }
  return bin_str;
}
