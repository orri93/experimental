#include <boost/test/auto_unit_test.hpp>

extern boost::unit_test::test_suite *GetBinaryStreamFixtureSuite();

boost::unit_test::test_suite* init_unit_test_suite(int /* argc */, char* /* argv */[])
{
  boost::unit_test::test_suite* binaryStreamSuiteTests = BOOST_TEST_SUITE("Binary Stream test suite");

  boost::unit_test::ut_detail::auto_test_unit_registrar(1);
  binaryStreamSuiteTests->add(GetBinaryStreamFixtureSuite());

  return binaryStreamSuiteTests;
}