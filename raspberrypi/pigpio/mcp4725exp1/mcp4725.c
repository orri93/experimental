#include <pigpio.h>

#include "mcp4725.h"

typedef union _I2cData_ {
  uint16_t value;     /* value */
  struct {
    uint8_t loByte;   /* low byte */
    uint8_t hiByte;   /* high byte */
  };
} I2cData;

void mcp4725_write(int i2ch, uint16_t value) {
  unsigned char buffer[2];
  I2cData data;

  data.value = value;
  buffer[0] = data.hiByte;
  buffer[1] = data.loByte;

  i2cWriteDevice(i2ch, buffer, 2);
}
