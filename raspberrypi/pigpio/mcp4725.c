#include <pigpio.h>

#include "mcp4725.h"

static union I2cData {
  uint16_t value;  /**< value */
  struct {
    uint8_t loByte;  /**< low byte */
    uint8_t hiByte;  /**< high byte */
  };
};

void mcp4725_write(int i2ch, uint16_t value) {
  unsigned char data[2];
  I2cData data;

  data.value = value;
  data[0] = data.loByte;
  data[1] = data.hiByte;

  i2cWriteDevice(i2ch, data, 2);
}
