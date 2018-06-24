// BinaryStreamFixture.cpp : Defines the entry point for the console application.
//

#include <boost/test/unit_test.hpp>
#include "FourBitNumberProtocol.h"

#ifdef __cplusplus
#define BINARY_STREAM_PREFIX fds_four_bit_number_stream_protocol
#else
#define BINARY_STREAM_PREFIX 
#endif

struct BinaryStreamFixture
{
  void TestSplitWordToTwoByte()
  {
    BINARY_STREAM_PREFIX::ffbnsp_ui16 input;
    BINARY_STREAM_PREFIX::ffbnsp_ui8 outputLow, outputHigh;

    input = 0x1234;
    BINARY_STREAM_PREFIX::ffbsnp_SplitWordToTwoByte(input, &outputLow, &outputHigh);
    BOOST_CHECK_EQUAL(0x12, outputHigh);
    BOOST_CHECK_EQUAL(0x34, outputLow);
  }

  void TestSplitByteToTwoFourBit()
  {
    BINARY_STREAM_PREFIX::ffbnsp_ui8 input, outputLow, outputHigh;

    input = 0x23;
    BINARY_STREAM_PREFIX::ffbsnp_SplitByteToTwoFourBit(input, &outputLow, &outputHigh);
    BOOST_CHECK_EQUAL(0x2, outputHigh);
    BOOST_CHECK_EQUAL(0x3, outputLow);
  }
};

boost::unit_test::test_suite* GetBinaryStreamFixtureSuite()
{
  boost::unit_test::test_suite* suite = BOOST_TEST_SUITE("Binary Stream Fixture Suite");

  boost::shared_ptr <BinaryStreamFixture> instance(new BinaryStreamFixture);
  suite->add(BOOST_CLASS_TEST_CASE(&BinaryStreamFixture::TestSplitWordToTwoByte, instance));
  suite->add(BOOST_CLASS_TEST_CASE(&BinaryStreamFixture::TestSplitByteToTwoFourBit, instance));

  return suite;
}
