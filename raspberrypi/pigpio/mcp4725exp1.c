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

/* MCP4725 Constant */
#define MCP4725_MAXVALUE        4095
#define MCP4725_MIDPOINT        2048

/* MCP4725 Errors */
#define MCP4725_OK              0
#define MCP4725_VALUE_ERROR     -999
#define MCP4725_REG_ERROR       -998
#define MCP4725_NOT_CONNECTED   -997

/* MCP4725 Power Down Mode */
#define MCP4725_PDMODE_NORMAL   0x00
#define MCP4725_PDMODE_1K       0x01
#define MCP4725_PDMODE_100K     0x02
#define MCP4725_PDMODE_500K     0x03

typedef struct {
  unsigned int bus;
  uint8_t address;
  uint16_t lastvalue;
  uint8_t powerdownmode;
  uint32_t lastwriteeeprom;
  double maxvoltage;
  int lasterror;
} MCP4725;

/* MCP4725 Function Prototypes */
void mcp4725init(MCP4725 *mcp4725, unsigned int bus, uint8_t address);
bool mcp4725begin(MCP4725 *mcp4725, double voltage);
bool mcp4725isconnected(MCP4725 *mcp4725);
bool mcp4725isready(MCP4725 *mcp4725);
int mcp4725setvalue(MCP4725 *mcp4725, const uint16_t value);

/* MCP4725 Implementation details */
int mcp4725writefastmode(MCP4725 *mcp4725, const uint16_t value);
int mcp4725writeregistermode(MCP4725 *mcp4725, const uint16_t value, uint8_t reg);
uint8_t mcp4725readregister(MCP4725 *mcp4725, uint8_t* buffer, const uint8_t length);
int mcp4725generalcall(MCP4725 *mcp4725, const uint8_t gc);

int main(int argc, char *argv[]) {
  int ret;

  MCP4725 mcp4725;
  
  /* Initialize MCP4725 on I2C bus 1 and address 0x62 *
   *   I2C bus 1 pins are                             *
   *     SDA: GPIO2                                   *
   *     SCL: GPIO3                                   */
  mcp4725init(&mcp4725, 1, 0x62);

  ret = gpioInitialise();
  if (ret < 0) {
    fprintf(stderr, "GPIO Initializing failed with error code: %d\n", ret);
    return EXIT_FAILURE;
  }

  if (!mcp4725begin(&mcp4725, 3.3)) {
    fprintf(stderr, "MCP4725 not connected\n");
    return EXIT_FAILURE;
  }

  printf("Terminating the GPIO\n");
  gpioTerminate();

  return EXIT_SUCCESS;
}

void mcp4725init(MCP4725 *mcp4725, unsigned int bus, uint8_t address) {
  mcp4725->bus = bus;
  mcp4725->address = address;
  mcp4725->lastvalue = 0;
  mcp4725->powerdownmode = MCP4725_PDMODE_NORMAL;
  mcp4725->lastwriteeeprom = 0;
  mcp4725->maxvoltage = 5.0;
  mcp4725->lasterror = 0;
}

bool mcp4725begin(MCP4725 *mcp4725, double voltage) {
  if ((mcp4725->address < 0x60) || (mcp4725->address > 0x67)) {
    return false;
  }

  if (!mcp4725isconnected(mcp4725)) {
    return false;
  }

  return true;
}

bool mcp4725isconnected(MCP4725 *mcp4725) {
  int i2ch;

  /* wire->beginTransmission(address)    */
  i2ch = i2cOpen(mcp4725->bus, mcp4725->address, 0);
  if (i2ch < 0) {
    mcp4725->lasterror = i2ch;
    return false;
  }

  /* return wire->endTransmission() == 0 */
  i2cClose(i2ch);

  return true;
}

bool mcp4725isready(MCP4725 *mcp4725) {

}

int mcp4725setvalue(MCP4725 *mcp4725, const uint16_t value) {
  if (value == mcp4725->lastvalue) {
    return MCP4725_OK;
  }
  if (value > MCP4725_MAXVALUE) {
    return MCP4725_VALUE_ERROR;
  }
  
  int rv = mcp4725writefastmode(mcp4725, value);

  if (rv == MCP4725_OK) {
    mcp4725->lastvalue = value;
  }

  return rv;
}

int mcp4725writefastmode(MCP4725 *mcp4725, const uint16_t value) {
  uint8_t l = value & 0xFF;
  uint8_t h = ((value / 256) & 0x0F);  /* set C0 = C1 = 0, no PDmode */
  h = h | (mcp4725->powerdownmode << 4);
}

int mcp4725writeregistermode(MCP4725 *mcp4725, const uint16_t value, uint8_t reg) {

}

/* Read the register value. See datasheet page 20. Typical 3 or 5 bytes. */
uint8_t mcp4725readregister(MCP4725 *mcp4725, uint8_t* buffer, const uint8_t length) {
  int i2ch;

  /* wire->beginTransmission(address) */
  i2ch = i2cOpen(mcp4725->bus, mcp4725->address, 0);
  if (i2ch < 0) {
    return i2ch;
  }


}

/* General Call (the name comes from the datasheet) */
int mcp4725generalcall(MCP4725 *mcp4725, const uint8_t gc) {
  int i2ch, wbr;

  /* wire->beginTransmission(0) */
  i2ch = i2cOpen(mcp4725->bus, 0, 0);
  if (i2ch < 0) {
    return i2ch;
  }
  
  /* wire->write(gc) */
  wbr = i2cWriteByte(i2ch, gc);
  if (wbr < 0) {
    i2cClose(i2ch);
    return wbr;
  }

  /* wire->endTransmission() */
  i2cClose(i2ch);

  return 0;
}
