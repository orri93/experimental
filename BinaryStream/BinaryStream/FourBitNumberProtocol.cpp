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

size_t ffbsnp_CalculageFourBitLenghtForByteBuffer(size_t byteLenght)
{
  return byteLenght * 2;
}

size_t ffbsnp_CalculateFourBitLengthForWordBuffer(size_t wordLength)
{
  return wordLength * 4;
}

size_t ffbsnp_Package(ffbnsp_ui16* buffer, size_t wordLength, ffbnsp_ui8* output, size_t outputSize)
{
  ffbnsp_ui8 low8, high8, low8low4, low8high4

  for (size_t i = 0; i < wordLength; i++)
  {

  }
}

#ifdef __cplusplus
}       /* fds_four_bit_number_stream_protocol */
#endif