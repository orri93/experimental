#ifndef _MCP320X_H_
#define _MCP320X_H_

#include <stdint.h>

/**
 * The ADC has 1 fixed non configurable input.
 */
enum MCP3201_CHANNEL {
  MCP3201_SINGLE_CH0 = 0x0
};

/**
 * The ADC has 2 inputs, usable as 2 individually single inputs with
 * reference to analog ground or 1 differential input. The polarity
 * of each differential input can be configured.
 *
 * bits|  2  | 1
 * :--:|:---:|:-:
 *  -  |type |ch
 *
 *  -  |      description
 * :--:|:--------------------------
 * type| 1 bit for single(1) or differential(0) input
 * ch  | 1 bit for channel number selection
 */
enum MCP3202_CHANNEL {
  MCP3202_SINGLE_0 = 0b10,  /**< single channel 0 */
  MCP3202_SINGLE_1 = 0b11,  /**< single channel 1 */
  MCP3202_DIFF_0PN = 0b00,  /**< differential channel 0 (input 0+,1-) */
  MCP3202_DIFF_0NP = 0b01,  /**< differential channel 0 (input 0-,1+) */
};

/**
 * The ADC has 4 inputs, usable as 4 individually single inputs with
 * reference to analog ground or 2 differential inputs. The polarity
 * of each differential input can be configured.
 *
 * bits|  4  | 3 | 2 | 1
 * :--:|:---:|:-:|:-:|:-:
 *  -  |type | x |ch |ch
 *
 *  -  |      description
 * :--:|:--------------------------
 * type| 1 bit for single(1) or differential(0) input
 * x   | 1 bit unused
 * ch  | 2 bit for channel number selection
 */
enum MCP3204_CHANNEL {
  MCP3204_SINGLE_0 = 0b1000,  /**< single channel 0 */
  MCP3204_SINGLE_1 = 0b1001,  /**< single channel 1 */
  MCP3204_SINGLE_2 = 0b1010,  /**< single channel 2 */
  MCP3204_SINGLE_3 = 0b1011,  /**< single channel 3 */
  MCP3204_DIFF_0PN = 0b0000,  /**< differential channel 0 (input 0+,1-) */
  MCP3204_DIFF_0NP = 0b0001,  /**< differential channel 0 (input 0-,1+) */
  MCP3204_DIFF_1PN = 0b0010,  /**< differential channel 1 (input 2+,3-) */
  MCP3204_DIFF_1NP = 0b0011   /**< differential channel 1 (input 2-,3+) */
};

/**
 * The ADC has 8 inputs, usable as 8 individually single inputs with
 * reference to analog ground or 4 differential inputs. The polarity
 * of each differential input can be configured.
 *
 * bits|  4  | 3 | 2 | 1
 * :--:|:---:|:-:|:-:|:-:
 *  -  |type |ch |ch |ch
 *
 *  -  |      description
 * :--:|:--------------------------
 * type| 1 bit for single(1) or differential(0) input
 * ch  | 3 bit for channel number selection
 */
enum MCP3208_CHANNEL {
  MCP3208_SINGLE_0 = 0b1000,  /**< single channel 0 */
  MCP3208_SINGLE_1 = 0b1001,  /**< single channel 1 */
  MCP3208_SINGLE_2 = 0b1010,  /**< single channel 2 */
  MCP3208_SINGLE_3 = 0b1011,  /**< single channel 3 */
  MCP3208_SINGLE_4 = 0b1100,  /**< single channel 4 */
  MCP3208_SINGLE_5 = 0b1101,  /**< single channel 5 */
  MCP3208_SINGLE_6 = 0b1110,  /**< single channel 6 */
  MCP3208_SINGLE_7 = 0b1111,  /**< single channel 7 */
  MCP3208_DIFF_0PN = 0b0000,  /**< differential channel 0 (input 0+,1-) */
  MCP3208_DIFF_0NP = 0b0001,  /**< differential channel 0 (input 0-,1+) */
  MCP3208_DIFF_1PN = 0b0010,  /**< differential channel 1 (input 2+,3-) */
  MCP3208_DIFF_1NP = 0b0011,  /**< differential channel 1 (input 2-,3+) */
  MCP3208_DIFF_2PN = 0b0100,  /**< differential channel 2 (input 4+,5-) */
  MCP3208_DIFF_2NP = 0b0101,  /**< differential channel 2 (input 5-,5+) */
  MCP3208_DIFF_3PN = 0b0110,  /**< differential channel 3 (input 6+,7-) */
  MCP3208_DIFF_3NP = 0b0111   /**< differential channel 3 (input 6-,7+) */
};

enum MCP320X_TYPE {
  MCP320X_MCP3201 = 1,
  MCP320X_MCP3202 = 2,
  MCP320X_MCP3204 = 4,
  MCP320X_MCP3208 = 8
};

typedef struct {
  unsigned channel;
  unsigned baud;
  unsigned flags;
} Mcp320xSpi;

typedef struct {
  Mcp320xSpi spi;
  int type;
} MCP320X;

void mcp320x_init(MCP320X* mcp320x, Mcp320xSpi* spi, int type);
uint16_t mcp320x_read(MCP320X* mcp320x, uint16_t ch);

#endif /* _MCP320X_H_ */
