/*
 * Raspberry Pi MCP4725 experiment 2 using C PiGPIO library
 *
 * Compile:
 *   gcc -Wall -pthread -o mcp4725exp2 mcp4725exp2.c -lpigpio
 *
 * Usage:
 *   ./mcp4725exp2
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
#define MCP4725_BUS         1
#define MCP4725_ADDRESS  0x62
#define MCP4725_FLAGS       0

#define MCP4725_MAXIMUM 0xfff

typedef union _I2cData_ {
  uint16_t value;     /* value */
  struct {
    uint8_t loByte;   /* low byte */
    uint8_t hiByte;   /* high byte */
  };
} I2cData;

void i2c_data_to_buffer_1(unsigned char* destination, uint16_t value);
void i2c_data_to_buffer_2(unsigned char* destination, uint16_t value);

int main(int argc, char *argv[]) {
  int ret, i2ch;
  unsigned char buffer[8];
  uint16_t data[4];


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

  data[0] = 0;
  data[1] = MCP4725_MAXIMUM / 2;
  data[2] = MCP4725_MAXIMUM;
  data[3] = 0;

  i2c_data_to_buffer_2(&(buffer[0]), data[0]);
  i2c_data_to_buffer_2(&(buffer[2]), data[1]);
  i2c_data_to_buffer_2(&(buffer[4]), data[2]);
  i2c_data_to_buffer_2(&(buffer[6]), data[3]);

  /* Works */
  i2cWriteDevice(i2ch, (char*)buffer, 8);
  
  /*
   * The following does not work (the high and low bites are incorrect)
   *
   *
  i2cWriteDevice(i2ch, (char*)data, 8);  */

  i2cClose(i2ch);

  printf("Terminating the GPIO\n");
  gpioTerminate();

  return EXIT_SUCCESS;
}

void i2c_data_to_buffer_1(unsigned char* destination, uint16_t value) {
  I2cData data;
  data.value = value;
  *destination = data.hiByte;
  destination++;
  *destination = data.loByte;
}

void i2c_data_to_buffer_2(unsigned char* destination, uint16_t value) {
  *destination = value >> 8;
  destination++;
  *destination = value & 0xff;
}

