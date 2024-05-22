/*
 * Raspberry Pi MCP4725 experiment 4 using C PiGPIO library
 *
 * Compile:
 *   gcc -Wall -pthread -o mcp4725exp4 mcp4725exp4.c -lm -lpigpio
 *
 * Usage:
 *   ./mcp4725exp4
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <pigpio.h>

/*
 * I2C bus 1 pins are
 *   SDA: GPIO2
 *   SCL: GPIO3
 *
 * The A0 pin on the MCP4725 is used to change the address
 *   Normally the address is 0x62
 *   But when A0 is connected to VDD, the address becomes 0x63
 */
#define MCP4725_BUS               1
#define MCP4725_ADDRESS        0x62
#define MCP4725_FLAGS             0

#define MCP4725_MAXIMUM       0xfff

#define DATA_COUNT         3 * 1024
#define BUFFER_SIZE  DATA_COUNT * 2

void i2c_data_to_buffer(uint8_t* destination, const uint16_t* values, int count);

int main(int argc, char *argv[]) {
  int ret, i2ch, i, v;
  double x, y;
  unsigned char buffer[BUFFER_SIZE];
  uint16_t data[DATA_COUNT];

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

  printf("x\ty\tv\n");
  for (i = 0; i < DATA_COUNT; i++) {
    x = i / 6.0;
    y = (sin(x) + 1.0) * MCP4725_MAXIMUM / 2.0;
    v = (int)y;
    printf("%f\t%f\t%d\n", x, y, v);
    data[i] = v;
  }

  i2c_data_to_buffer(buffer, data, DATA_COUNT);

  while (1) {
  ret = i2cWriteDevice(i2ch, (char*)buffer, BUFFER_SIZE);
  if (ret < 0) {
    switch(ret) {
    case PI_BAD_HANDLE:
      fprintf(stderr, "I2C Write failed because of a bad handle\n");
      break;
    case PI_BAD_PARAM:
      fprintf(stderr, "I2C Write failed because of a bad parameters\n");
      break;
    case PI_I2C_WRITE_FAILED:
      fprintf(stderr, "I2C Write failed\n");
      break;
    default:
      fprintf(stderr, "I2C Write failed with the error code: %d\n", ret);
      break;
    }
  }
  }

  i2cClose(i2ch);

  printf("Terminating the GPIO\n");
  gpioTerminate();

  return EXIT_SUCCESS;
}

void i2c_data_to_buffer(uint8_t* destination, const uint16_t* values, int count) {
  int i;
  uint16_t value;
  for (i = 0; i < count; i++) {
    value = values[i];
    if (value > MCP4725_MAXIMUM) {
      value = MCP4725_MAXIMUM;
      fprintf(stderr, "Value %d at #%d is greater than the maximum value %d\n", values[i], i, MCP4725_MAXIMUM);
    }
    *destination  = value >> 8;
    destination++;
    *destination = value & 0xff;
    destination++;
  }
}

