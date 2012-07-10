#include <config/config.hpp>
#include <logging/logging.hpp>
#include <mpreal/mpreal.h>

#define BOOST_TEST_MODULE mpreal
#include <boost/test/included/unit_test.hpp>

using namespace mpfr;

BOOST_AUTO_TEST_SUITE(mpreal_suite1)

BOOST_AUTO_TEST_CASE(mpreal_test1)
{
  mpreal x = "12";
  mpreal y = "9898798787.878787";
  mpreal z = "418.9829";
  mpreal res = "418.9829"*x/x*y/y;
  BOOST_REQUIRE(res == z);
  log_debug(res.toString());
}

BOOST_AUTO_TEST_SUITE_END()
