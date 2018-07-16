#ifndef FDS_BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_H_
#define FDS_BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
namespace fds_four_bit_number_stream_protocol
{
#endif

/* Lower four bit mask */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_LOWER_FOUR_BIT_MASK        0x0f    /* Bin: 00 00 1111 */

/* Control is the first two bit and start have them set to zero and then create unique pattern: 0x31 */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_START                      0x31    /* Bin: 00 11 0001 */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_MASK               0xc0    /* Bin: 11 00 0000 */
#define BINAEY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_COTNROL_SUB_MASK           0x30    /* Bin: 00 11 0000 */

/* Control is of the type subtype is count */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_COUNT         0x40    /* Bin: 01 00 nnnn */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_COUNT_MASK    0x0f    /* Bin: 00 00 1111 */

/* Control is of the type length and the next 6 bits are all used providing maximum 63 */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_LENGTH        0x80    /* Bin: 10 nn nnnn */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_LENGTH_MASK   0x3f    /* Bin: 00 11 1111 */

/* Control is of the type subtype is value type and the lower 4 bits are the type */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_VALUE         0x70    /* Bin: 01 11 nnnn */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_TYPE_VALUE_MASK    0x0f    /* Bin: 00 00 1111 */

/* Control is of the type value lenght and the lower 4 bits are the length ex WORD: 4 + 1 (chekcsum) = 5 */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_VALUE_LENGTH       0x80    /* Bin: 10 00 nnnn */
#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_CONTROL_VALUE_LENGTH_MASK  0x0f    /* Bin: 00 00 nnnn */

#define BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_PACKAGE_VALUE_OR_CHECKSUM          0xc0    /* Bin: 11 nn vvvv or 11 ss ssss */

typedef uint16_t ffbnsp_ui16;
typedef uint8_t ffbnsp_ui8;
typedef signed int ffbnsp_size;
typedef int ffbnsp_int;

void ffbsnp_SplitWordToTwoByte(ffbnsp_ui16 value, ffbnsp_ui8* low, ffbnsp_ui8* high);
void ffbsnp_SplitByteToTwoFourBit(ffbnsp_ui8 value, ffbnsp_ui8* low, ffbnsp_ui8* high);

ffbnsp_ui8 ffbsnp_CombineTwoFourBitToByte(ffbnsp_ui8 low, ffbnsp_ui8 high);
ffbnsp_ui16 ffbsnp_CombineTwoByteToWord(ffbnsp_ui8 low, ffbnsp_ui8 high);

ffbnsp_size ffbsnp_CalculageFourBitLenghtForByteBuffer(ffbnsp_size wordLength);
ffbnsp_size ffbsnp_CalculateFourBitLengthForWordBuffer(ffbnsp_size wordLength);

ffbnsp_size ffbsnp_BytePackage(
  ffbnsp_ui8* buffer,
  ffbnsp_size byteLenght,
  ffbnsp_ui8* output,
  ffbnsp_size outputReservedSize,
  ffbnsp_size startPosition,
  ffbnsp_int* checksum
  );
ffbnsp_size ffbsnp_WordPackage(
  ffbnsp_ui16* buffer,
  size_t wordLength,
  ffbnsp_ui8* output,
  ffbnsp_size outputReservedSize,
  ffbnsp_size startPosition,
  ffbnsp_int* checksum
  );

ffbnsp_size ffbsnp_CalculatePackageHeaderSize(ffbnsp_int valueCount, ffbnsp_int* valueLenght);
ffbnsp_size ffbsnp_CalculatePackagePayloadSize(ffbnsp_int valueCount, ffbnsp_int* valueLength);

ffbnsp_size ffbsnp_CreatePackageHeader(
  ffbnsp_int valueCount,
  ffbnsp_int* valueLength,
  ffbnsp_ui8* output,
  ffbnsp_size outputReservedSize
  );
ffbnsp_size ffbsnp_CreatePackagePayloade(
  ffbnsp_int valueCount,
  ffbnsp_int* valueLength,
  ffbnsp_int *valueType,
  ffbnsp_int *value,
  ffbnsp_ui8* output,
  ffbnsp_size outputReservedSize
  );

#ifdef __cplusplus
}       /* fds_four_bit_number_stream_protocol */
#endif

#endif  /* FDS_BINARY_STREAM_FOUR_BIT_NUMBER_PROTOCOL_H_ */