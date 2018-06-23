#ifndef FDS_BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_H_
#define FDS_BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
namespace fds_four_bit_number_stream_protocol
{
#endif

typedef uint16_t ffbnsp_ui16;
typedef uint8_t ffbnsp_ui8;
typedef size_t ffbnsp_size;

void ffbsnp_SplitWordToTwoByte(ffbnsp_ui16 value, ffbnsp_ui8* low, ffbnsp_ui8* high);
void ffbsnp_SplitByteToTwoFourBit(ffbnsp_ui8 value, ffbnsp_ui8* low, ffbnsp_ui8* high);

ffbnsp_ui8 ffbsnp_CombineTwoFourBitToByte(ffbnsp_ui8 low, ffbnsp_ui8 high);
ffbnsp_ui16 ffbsnp_CombineTwoByteToWord(ffbnsp_ui8 low, ffbnsp_ui8 high);

ffbnsp_size ffbsnp_CalculageFourBitLenghtForByteBuffer(ffbnsp_size wordLength);
ffbnsp_size ffbsnp_CalculateFourBitLengthForWordBuffer(ffbnsp_size wordLength);

ffbnsp_size ffbsnp_BytePackage(ffbnsp_ui8* buufer, ffbnsp_size byteLenght, ffbnsp_ui8* output, size_t outputReservedSize);
ffbnsp_size ffbsnp_WordPackage(ffbnsp_ui16* buffer, size_t wordLength, ffbnsp_ui8* output, size_t outputReservedSize);

#ifdef __cplusplus
}       /* fds_four_bit_number_stream_protocol */
#endif

#endif  /* FDS_BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_H_ */