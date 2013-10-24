#include <config/config.hpp>
#include <logging/logging.hpp>
#include <fixmath/fix16.hpp>
#include <world/vec2.hpp>

#define BOOST_TEST_MODULE fixmath
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(fixmath_suite1)

BOOST_AUTO_TEST_CASE(fixmath_test1)
{
  Fix16 x = 12;
  Fix16 y = 9898.88787;
  // Fix16 z = "418.9829";
  // Fix16 res = "418.9829"*x/x*y/y;
  // BOOST_REQUIRE(res == z);

  Vec2 c(0, 1);
  for (int i = 0; i < 63; ++i)
    {
      log_debug(c << ": " << c.length());
      c.rotate(-0.1);
    }
  // log_debug(atan2f(c.x.toDouble(), c.y.toDouble()));
}

BOOST_AUTO_TEST_SUITE_END()
