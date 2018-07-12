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

ffbnsp_size ffbsnp_BytePackage(
  ffbnsp_ui8* buffer,
  ffbnsp_size byteLenght,
  ffbnsp_ui8* output,
  ffbnsp_size outputReservedSize,
  ffbnsp_size startPosition,
  int* checksum)
{
  ffbnsp_ui8 value, lowFourBit, highFourBit;
  ffbnsp_size currentOutputPossition = startPosition;

  if (outputReservedSize < 2)
  {
    return -1;
  }

  *checksum = 0;

  for (ffbnsp_size i = 0; i < byteLenght; i++)
  {
    value = buffer[i];
    ffbsnp_SplitByteToTwoFourBit(value, &lowFourBit, &highFourBit);
    lowFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_VALUE_OR_CHECKSUM | 0x00;
    highFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_VALUE_OR_CHECKSUM | 0x10;

    *checksum ^= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_LOWER_FOUR_BIT_MASK & lowFourBit;
    *checksum ^= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_LOWER_FOUR_BIT_MASK & highFourBit;

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

ffbnsp_size ffbsnp_WordPackage(
  ffbnsp_ui16* buffer,
  ffbnsp_size wordLength,
  ffbnsp_ui8* output,
  ffbnsp_size outputReservedSize,
  ffbnsp_size startPosition,
  int* checksum
  )
{
  ffbnsp_ui8 lowByte, highByte, lowByteLowFourBit, lowByteHighFourBit, highByteLowFourBit, highByteHighFourBit;
  ffbnsp_ui16 value;

  ffbnsp_size currentOutputPossition = startPosition;

  if (outputReservedSize < 4)
  {
    return -1;
  }

  *checksum = 0;

  for (ffbnsp_size i = 0; i < wordLength; i++)
  {
    value = buffer[i];
    ffbsnp_SplitWordToTwoByte(value, &lowByte, &highByte);
    ffbsnp_SplitByteToTwoFourBit(lowByte, &lowByteLowFourBit, &lowByteHighFourBit);
    ffbsnp_SplitByteToTwoFourBit(highByte, &highByteLowFourBit, &highByteHighFourBit);
    
    // Specify it is a numbered stream and include count
    lowByteLowFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_VALUE_OR_CHECKSUM | 0x00;
    lowByteHighFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_VALUE_OR_CHECKSUM | 0x10;
    highByteLowFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_VALUE_OR_CHECKSUM | 0x20;
    highByteHighFourBit |= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_VALUE_OR_CHECKSUM | 0x30;

    *checksum ^= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_LOWER_FOUR_BIT_MASK & lowByteLowFourBit;
    *checksum ^= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_LOWER_FOUR_BIT_MASK & lowByteHighFourBit;
    *checksum ^= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_LOWER_FOUR_BIT_MASK & highByteLowFourBit;
    *checksum ^= BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_LOWER_FOUR_BIT_MASK & highByteHighFourBit;

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

ffbnsp_size ffbsnp_CalculatePackageHeaderSize(int valueCount, int* valueLenght)
{
  ffbnsp_size headerSize = (
    1 + // Package begins
    1 + // Number of values (type)
    1   // Payload length in bytes
    );
  return headerSize;
}

ffbnsp_size ffbsnp_CalculatePackagePayloadSize(int valueCount, int* valueLength)
{
  ffbnsp_size payloadSize = 0;
  for (int i = 0; i < valueCount; i++)
  {
    int vlen = valueLength[i];
    payloadSize += (
      2 +     // Type and length
      vlen +  // Actual value
      1       // Value checksum
      );
  }
  return
    payloadSize +
    1               // Total checksum
    ;
}

ffbnsp_size ffbsnp_CreatePackageHeader(int valueCount, int* valueLength, ffbnsp_ui8* output, ffbnsp_size outputReservedSize)
{
  ffbnsp_size headerSize = ffbsnp_CalculatePackageHeaderSize(valueCount, valueLength);
  if (outputReservedSize < headerSize)
  {
    return -1;
  }

  if (valueCount > BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_COUNT_MASK)
  {
    return -1;
  }

  ffbnsp_size payloadSize = ffbsnp_CalculatePackagePayloadSize(valueCount, valueLength);
  if (payloadSize > BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_LENGTH_MASK)
  {
    return -1;
  }

  int index = 0;
  output[index++] = BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_START;
  output[index++] = BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_COUNT |
    (((ffbnsp_ui8)valueCount) & BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_COUNT_MASK);
  output[index++] = BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_LENGTH |
    (((ffbnsp_ui8)payloadSize) & BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_LENGTH_MASK);
  return index;
}

ffbnsp_size ffbsnp_CreatePackagePayloade(
  int valueCount,
  int* valueLength,
  int *valueType,
  int *value,
  ffbnsp_ui8* output,
  ffbnsp_size outputReservedSize
  )
{
  int i, index, checksum, totalchecksum, type, length;
  ffbnsp_ui8  buffer8[1];
  ffbnsp_ui16 buffer16[1];

  ffbnsp_size payloadSize = ffbsnp_CalculatePackagePayloadSize(valueCount, valueLength);
  if (outputReservedSize < payloadSize)
  {
    return -1;
  }
  
  index = 0;
  checksum = 0;
  totalchecksum = 0;
  for (i = 0; i < valueCount; i++)
  {
    type = valueType[i];
    if (type > BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_VALUE_MASK)
    {
      return -1;
    }
    output[index++] = BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_VALUE |
      (((ffbnsp_ui8)type) & BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_VALUE_MASK);
    length = valueLength[i];
    if (length > BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_VALUE_LENGTH_MASK)
    {
      return -1;
    }
    output[index++] = BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_VALUE_LENGTH |
      (((ffbnsp_ui8)length) & BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_VALUE_LENGTH_MASK);
    checksum = 0;
    switch (length)
    {
    case 2:
      buffer8[0] = (ffbnsp_ui8)(value[i]);
      index = ffbsnp_BytePackage(buffer8, 1, output, outputReservedSize, index, &checksum);
      break;
    case 4:
      buffer16[0] = (ffbnsp_ui16)(value[i]);
      index = ffbsnp_WordPackage(buffer16, 1, output, outputReservedSize, index, &checksum);
      break;
    default:
      return -1;
    }
    totalchecksum ^= checksum;
  }

}

#ifdef __cplusplus
}       /* fds_four_bit_number_stream_protocol */
#endif