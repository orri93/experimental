#include <string.h>

#include <pigpio.h>

#include "mcp320x.h"

static union SpiData {
  uint16_t value;  /**< value */
  struct {
    uint8_t loByte;  /**< low byte */
    uint8_t hiByte;  /**< high byte */
  };
};

static uint16_t mcp320x_create_command(MCP320X* mcp320x, uint16_t ch);
static uint16_t mcp320x_transfer_00(Mcp320xSpi* spi);
static uint16_t mcp320x_transfer_01(Mcp320xSpi* spi);
static uint16_t mcp320x_transfer_10(Mcp320xSpi* spi, SpiData* cmd);
static uint16_t mcp320x_transfer_11(Mcp320xSpi* spi, SpiData* cmd);

void mcp320x_init(MCP320X* mcp320x, Mcp320xSpi* spi, MCP320X_TYPE type, uint16_t vref) {
  mcp320x->spi.channel = spi->channel;
  mcp320x->spi.baud = spi->baud;
  mcp320x->spi.flags = spi->flags;
  mcp320x->type = type;
  mcp320x->vref = vref;
  mcp320x->speed = 0;
}

uint16_t mcp320x_read(MCP320X* mcp320x, uint16_t ch) {
  uint16_t value;
  SpiData cmd;

  cmd.value = mcp320x_create_command(mcp320x, ch);

  switch (mcp320x->type) {
    case MCP3201:
      return mcp320x_transfer_01(mcp320x->spi);
    case MCP3202:
      return mcp320x_transfer_11(mcp320x->spi, &cmd);
    case MCP3204:
      return mcp320x_transfer_11(mcp320x->spi, &cmd);
    case MCP3208:
      return mcp320x_transfer_11(mcp320x->spi, &cmd);
    default:
      return 0;
  }
}

uint16_t mcp320x_create_command(MCP320X* mcp320x, uint16_t ch) {
  switch (mcp320x->type) {
    case MCP3201:
      /* no command required */
      return 0;
    case MCP3202:
      /* base command structure
       * 0b00000001cc100000
       * channel config */
      return 0x0120 | (ch << 6);
    case MCP3204:
      /* base command structure
       * 0b000001cxcc000000
       * channel config */
      return 0x0400 | (ch << 6);
    case MCP3208:
      /* base command structure
       * 0b000001cccc000000
       * channel config */
      return 0x0400 | (ch << 6);
    default:
      return 0x0;
  }
}

uint16_t mcp320x_transfer_00(Mcp320xSpi* spi) {
  SpiData adc;
  char sd, rd;
  int spih;

  sd = 0;

  spih = spiOpen(spi->channel, spi->baud, spi->flags);
  if (spih < 0) {
    return 0;
  }

  /* receive first(msb) 5 bits */
  spiXfer(spih, &sd, &rd, 1);
  adc.hiByte = rd & 0x1F;

  /* receive last(lsb) 8 bits */
  spiXfer(spih, &sd, &rd, 1);
  adc.loByte = rd;

  spiClose(spih);

  /* correct bit offset
   * |x|x|x|11|10|9|8|7| |6|5|4|3|2|1|0|1 */
  return adc.value >> 1;
}

uint16_t mcp320x_transfer_01(Mcp320xSpi* spi) {
  SpiData adc;
  char sd[2], rd[2];
  int spih;

  memset(sd, 0, 2);

  spih = spiOpen(spi->channel, spi->baud, spi->flags);
  if (spih < 0) {
    return 0;
  }

  spiXfer(spih, sd, rd, 2);

  spiClose(spih);

  adc.hiByte = rd[0] & 0x1F;
  adc.loByte = rd[1];

  /* correct bit offset
   * |x|x|x|11|10|9|8|7| |6|5|4|3|2|1|0|1 */
  return adc.value >> 1;
}

uint16_t mcp320x_transfer_10(Mcp320xSpi* spi, SpiData* spiData) {
  SpiData adc;
  char sd, rd;
  int spih;

  spih = spiOpen(spi->channel, spi->baud, spi->flags);
  if (spih < 0) {
    return 0;
  }

  /* send first command byte and ignore the recived data */
  sd = spiData->hiByte;
  spiXfer(spih, &sd, &rd, 1);

  /* send second command byte and receive first(msb) 4 bits */
  sd = spiData->loByte;
  spiXfer(spih, &sd, &rd, 1);
  adc.hiByte = rd & 0x0F;

  /* receive last(lsb) 8 bits */
  sd = 0;
  spiXfer(spih, &sd, &rd, 1);
  adc.loByte = rd;

  spiClose(spih);

  return adc.value;
}

uint16_t mcp320x_transfer_11(Mcp320xSpi* spi, SpiData* spiData) {
  SpiData adc;
  char sd[3], rd[3];
  int spih;

  sd[0] = spiData->hiByte;
  sd[1] = spiData->loByte;
  sd[2] = 0;

  spih = spiOpen(spi->channel, spi->baud, spi->flags);
  if (spih < 0) {
    return 0;
  }

  spiXfer(spih, sd, rd, 3);

  spiClose(spih);

  adc.hiByte = rd[1] & 0x0F;
  adc.loByte = rd[2];

  return adc.value;
}
