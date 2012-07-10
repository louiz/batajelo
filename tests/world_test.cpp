#include <config/config.hpp>
#include <logging/logging.hpp>
#include <world/map.hpp>
#include <gui/camera/map.hpp>
#include <pathfinding/astar.hpp>

#define BOOST_TEST_MODULE parser
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(parser_suite1)

BOOST_AUTO_TEST_CASE(parser_test1)
{
  GraphMap map;
  map.load_from_file("test2.tmx");
  int x;
  int y;
  get_cell_at_position(100, 180, 90, x, y);
}

BOOST_AUTO_TEST_SUITE_END()
