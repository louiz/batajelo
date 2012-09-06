#include <config/config.hpp>
#include <logging/logging.hpp>
#include <mpreal/mpreal.h>
#include <world/vec2.hpp>

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
  // log_debug(res.toString());
  // log_debug(atan2f(1., 0.));
  // mpreal x(1);

  Vec2 c(0, 1);
  for (int i = 0; i < 63; ++i)
    {
      log_debug(c << ": " << c.length());
      c.rotate(-0.1);
    }
  // log_debug(atan2f(c.x.toDouble(), c.y.toDouble()));

}

BOOST_AUTO_TEST_SUITE_END()
