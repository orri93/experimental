#include "FourBitNumberProtocol.h"

#ifdef __unix__
#elif defined(_WIN32) || defined(WIN32)
#include <SDKDDKVer.h>
#endif

#ifdef __cplusplus
namespace fds_four_bit_number_stream_protocol
{
#endif

void ffbsnp_SplitWordToTwoByte(ffbnsp_ui16 value, ffbnsp_ui8* low, ffbnsp_ui8* high)
{
  *low = value & 0xff;
  *high = (value >> 8);
}

void ffbsnp_SplitByteToTwoFourBit(ffbnsp_ui8 value, ffbnsp_ui8* low, ffbnsp_ui8* high)
{
  *low = value & 0x0f;
  *high = (value >> 4);
}

ffbnsp_ui8 ffbsnp_CombineTwoFourBitToByte(ffbnsp_ui8 low, ffbnsp_ui8 high)
{
  return low | (high << 4);
}

ffbnsp_ui16 ffbsnp_CombineTwoByteToWord(ffbnsp_ui8 low, ffbnsp_ui8 high)
{
  return ((ffbnsp_ui16)low) | (((ffbnsp_ui16)high) << 8);
}

ffbnsp_size ffbsnp_CalculageFourBitLenghtForByteBuffer(ffbnsp_size byteLenght)
{
  return byteLenght * 2;
}

ffbnsp_size ffbsnp_CalculateFourBitLengthForWordBuffer(ffbnsp_size wordLength)
{
  return wordLength * 4;
}



ffbnsp_size ffbsnp_BytePackage(ffbnsp_ui8* buffer, ffbnsp_size byteLenght, ffbnsp_ui8* output, size_t outputReservedSize)
{
  ffbnsp_ui8 value, lowFourBit, highFourBit;
  ffbnsp_size currentOutputPossition = 0;

  if (outputReservedSize < 2)
  {
    return -1;
  }

  for (ffbnsp_size i = 0; i < byteLenght; i++)
  {
    value = buffer[i];
    ffbsnp_SplitByteToTwoFourBit(value, &lowFourBit, &highFourBit);
    lowFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_NUMBER_BIT | 0x00;
    highFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_NUMBER_BIT | 0x01;

    output[currentOutputPossition] = lowFourBit;
    if (++currentOutputPossition >= outputReservedSize - 1) {
      return -1;
    }
    output[currentOutputPossition] = highFourBit;
    if (++currentOutputPossition >= outputReservedSize - 1) {
      return -1;
    }
  }
  return currentOutputPossition;
}

ffbnsp_size ffbsnp_WordPackage(ffbnsp_ui16* buffer, ffbnsp_size wordLength, ffbnsp_ui8* output, ffbnsp_size outputReservedSize)
{
  ffbnsp_ui8 lowByte, highByte, lowByteLowFourBit, lowByteHighFourBit, highByteLowFourBit, highByteHighFourBit;
  ffbnsp_ui16 value;

  ffbnsp_size currentOutputPossition = 0;

  if (outputReservedSize < 4)
  {
    return -1;
  }

  for (ffbnsp_size i = 0; i < wordLength; i++)
  {
    value = buffer[i];
    ffbsnp_SplitWordToTwoByte(value, &lowByte, &highByte);
    ffbsnp_SplitByteToTwoFourBit(lowByte, &lowByteLowFourBit, &lowByteHighFourBit);
    ffbsnp_SplitByteToTwoFourBit(highByte, &highByteLowFourBit, &highByteHighFourBit);
    
    // Specify it is a numbered stream and include count
    lowByteLowFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_NUMBER_BIT | 0x00;
    lowByteHighFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_NUMBER_BIT | 0x01;
    highByteLowFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_NUMBER_BIT | 0x02;
    highByteHighFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_NUMBER_BIT | 0x03;

    output[currentOutputPossition] = lowByteLowFourBit;
    if (++currentOutputPossition >= outputReservedSize - 1) {
      return -1;
    }
    output[currentOutputPossition] = lowByteHighFourBit;
    if (++currentOutputPossition >= outputReservedSize - 1) {
      return -1;
    }
    output[currentOutputPossition] = highByteLowFourBit;
    if (++currentOutputPossition >= outputReservedSize - 1) {
      return -1;
    }
    output[currentOutputPossition] = highByteHighFourBit;
    if (++currentOutputPossition >= outputReservedSize - 1) {
      return -1;
    }
  }
  return currentOutputPossition;
}



#ifdef __cplusplus
}       /* fds_four_bit_number_stream_protocol */
#endif