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

#define SPI_CHANNEL       0
#define SPI_SPEED   1000000

/* MCP3208 single-ended channel 0 to 7 */
#define MCP3208_SINGLE_CH0 0x8000
#define MCP3208_SINGLE_CH1 0x9000
#define MCP3208_SINGLE_CH2 0xA000
#define MCP3208_SINGLE_CH3 0xB000
#define MCP3208_SINGLE_CH4 0xC000
#define MCP3208_SINGLE_CH5 0xD000
#define MCP3208_SINGLE_CH6 0xE000
#define MCP3208_SINGLE_CH7 0xF000

const uint16_t channels[] = {
  MCP3208_SINGLE_CH0, MCP3208_SINGLE_CH1, MCP3208_SINGLE_CH2, MCP3208_SINGLE_CH3,
  MCP3208_SINGLE_CH4, MCP3208_SINGLE_CH5, MCP3208_SINGLE_CH6, MCP3208_SINGLE_CH7};

static char* binray_repr(uint16_t value);

int main(int argc, char *argv[]) {
  int spih, i;
  uint16_t send_data, receive_data;

  if (gpioInitialise() < 0) {
    fprintf(stderr, "Failed to initialize GPIO\n");
    return 1;
  }

  spih = spiOpen(SPI_CHANNEL, SPI_SPEED, 0);
  if (spih < 0) {
    fprintf(stderr, "Failed to open SPI channel\n");
    return 1;
  }

  for (i = 0; i < 8; i++) {
    send_data = channels[i];

    spiXfer(spih, (char*)&send_data, (char*)&receive_data, 2);

    printf("RAW Communication for channel %d; sending %s, receiving %x\n", i, binray_repr(send_data), receive_data);

    // The MCP3208's 12-bit ADC result is in the last 12 bits of receive_data.
    int adc_result = receive_data & 0x0FFF;

    printf("ADC result for channel %d: %d\n", i, adc_result);
  }

  spiClose(spih);
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
