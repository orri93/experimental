/*
 * Raspberry Pi MCP4725 experiment 1 using C PiGPIO library
 *
 * Compile:
 *   gcc -Wall -pthread -o mcp4725exp1 mcp4725exp1.c -lpigpio
 *
 * Usage:
 *   ./mcp4725exp1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <pigpio.h>

#include "mcp4725.h"

/*
 * I2C bus 1 pins are
 *   SDA: GPIO2
 *   SCL: GPIO3
 *
 * The A0 pin on the MCP4725 is used to change the address
 *   Normally the address is 0x62
 *   But when A0 is connected to VDD, the address becomes 0x63
 */
#define MCP4725_BUS         1
#define MCP4725_ADDRESS  0x62
#define MCP4725_FLAGS       0

#define MCP4725_MAXIMUM 0xfff

int main(int argc, char *argv[]) {
  int ret, i2ch, i;

  ret = gpioInitialise();
  if (ret < 0) {
    fprintf(stderr, "GPIO Initializing failed with the error code: %d\n", ret);
    return EXIT_FAILURE;
  }

  i2ch = i2cOpen(MCP4725_BUS, MCP4725_ADDRESS, MCP4725_FLAGS);
  if (i2ch < 0) {
    fprintf(stderr, "Failed to open I2C bus with the error code: %d\n", i2ch);
    return EXIT_FAILURE;
  }

  printf("Setting the DAC to zero\n");
  mcp4725_write(i2ch, 0);
  printf("Setting the DAC to half\n");
  mcp4725_write(i2ch, MCP4725_MAXIMUM / 2);
  printf("Setting the DAC to max\n");
  mcp4725_write(i2ch, MCP4725_MAXIMUM);
  printf("Setting the DAC to zero\n");
  mcp4725_write(i2ch, 0);  

  i2cClose(i2ch);

  printf("Terminating the GPIO\n");
  gpioTerminate();

  return EXIT_SUCCESS;
}
