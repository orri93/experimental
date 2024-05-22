/*
 * Raspberry Pi MCP4725 experiment 3 using C PiGPIO library
 *
 * Compile:
 *   gcc -Wall -pthread -o mcp4725exp3 mcp4725exp3.c -lpigpio
 *
 * Usage:
 *   ./mcp4725exp3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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

#define DATA_COUNT            16380
#define BUFFER_SIZE  DATA_COUNT * 2

void i2c_data_to_buffer(uint8_t* destination, const uint16_t* values, int count);

int main(int argc, char *argv[]) {
  int ret, i2ch, i, v, c;
  unsigned char buffer[BUFFER_SIZE];
  uint16_t data[DATA_COUNT];
  char* at;

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

  v = 0;
  for (i = 0; i < DATA_COUNT; i++) {
    if (v > MCP4725_MAXIMUM) {
      v = 0;
    }
    data[i] = v;
    v++;
  }

  i2c_data_to_buffer(buffer, data, DATA_COUNT);
  
  at = (char*)buffer;
  for (i = 0; i < 6; i++) {
    ret = i2cWriteDevice(i2ch, at, 1024);
    at += 1024;
  }

  c = 6 * 1024;
  if (c > BUFFER_SIZE) {
    fprintf(stderr, "Trying to write more bytes %d than the buffer size %d\n", c, BUFFER_SIZE);
    c = BUFFER_SIZE;
  }

  ret = i2cWriteDevice(i2ch, (char*)buffer, c);
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

