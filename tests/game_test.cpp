#include <config/config.hpp>
#include <logging/logging.hpp>
#include <boost/bind.hpp>
#include <game/turn_handler.hpp>
#include <game/action.hpp>

#define BOOST_TEST_MODULE game
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(turn_handler_suite1)

static int global_a = 0;

void coucou()
{
  global_a++;
}

BOOST_AUTO_TEST_CASE(turn_handler_1)
{
  TurnHandler t;
  Action* a = new Action(boost::bind(&coucou));
  BOOST_REQUIRE(t.insert_action(a, 0) == false);

  Action* a2 = new Action(boost::bind(&coucou));
  BOOST_REQUIRE(t.insert_action(a2, 1) == true);

  Action* a3 = new Action(boost::bind(&coucou));
  BOOST_REQUIRE(t.insert_action(a3, 1) == true);

  Action* a4 = new Action(boost::bind(&coucou));
  BOOST_REQUIRE(t.insert_action(a4, 2) == true);

  for (int i = 0; i < 100; ++i)
    t.tick();

  Action* a5 = new Action(boost::bind(&coucou));
  BOOST_REQUIRE(t.insert_action(a5, 14) == true);

  Action* a6 = new Action(boost::bind(&coucou));
  BOOST_REQUIRE(t.insert_action(a6, 4) == false);

  Action* a7 = new Action(boost::bind(&coucou));
  BOOST_REQUIRE(t.insert_action(a7, 14) == true);

  Action* a8 = new Action(boost::bind(&coucou));
  BOOST_REQUIRE(t.insert_action(a8, 14) == true);

  Action* a9 = new Action(boost::bind(&coucou));
  BOOST_REQUIRE(t.insert_action(a9, 15) == true);

  for (int i = 0; i < 100; ++i)
    t.tick();

  BOOST_REQUIRE(global_a == 7);

}

BOOST_AUTO_TEST_SUITE_END()
