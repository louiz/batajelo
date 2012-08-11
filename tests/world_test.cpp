#include <config/config.hpp>
#include <logging/logging.hpp>
#include <world/map.hpp>
#include <gui/camera/map.hpp>
#include <pathfinding/astar.hpp>
#include <world/position.hpp>

#define BOOST_TEST_MODULE world
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(world_suite1)

BOOST_AUTO_TEST_CASE(world_test1)
{
  Position p(12.2323232323, 23.2323);
  p += p;
  Position p2(12, 11);
  log_debug((p / 2) * 2);
  BOOST_REQUIRE(((p / 2878) * 2878) == p);
}

BOOST_AUTO_TEST_SUITE_END()
