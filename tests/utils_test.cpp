#include <utils/base64.hpp>
#include <utils/zlib.hpp>
#include <iostream>
#include <config/config.hpp>
#include <logging/logging.hpp>

#define BOOST_TEST_MODULE utils
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(utils_suite)

BOOST_AUTO_TEST_CASE(base64_test)
{
  std::string hello = "Coucou ici";
  BOOST_REQUIRE(base64_decode(
                  base64_encode(
                      reinterpret_cast<const unsigned char*>(hello.c_str()), hello.size())) == "Coucou ici");

  std::string encoded("TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");
  BOOST_REQUIRE(base64_decode(encoded) == "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.");
}

BOOST_AUTO_TEST_CASE(zlib_test)
{
  std::string original("coucou coucou");
  BOOST_REQUIRE(zlib_decompress(zlib_compress(original)) == "coucou coucou");
}


BOOST_AUTO_TEST_SUITE_END()