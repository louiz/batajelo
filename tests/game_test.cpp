#include <config/config.hpp>
#include <logging/logging.hpp>
#include <functional>
#include <game/turn_handler.hpp>
#include <game/turn.hpp>
#include <game/action.hpp>
#include <game/event.hpp>

#define BOOST_TEST_MODULE game
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(turn_handler_suite1)

static int global_a = 0;

void coucou(Event* event)
{
  (void)event;
  global_a++;
}

BOOST_AUTO_TEST_CASE(turn_handlerstd::placeholders::_1)
{
  global_a = 0;

  TurnHandler t;
  Action* a = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 1);
  BOOST_REQUIRE(t.insert_action(a, 0) == false);

  Action* a2 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 0);
  BOOST_REQUIRE(t.insert_action(a2, 1) == true);

  Action* a3 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 0);
  BOOST_REQUIRE(t.insert_action(a3, 2) == true);

  Action* a4 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 0);
  BOOST_REQUIRE(t.insert_action(a4, 1) == true);

  for (int i = 0; i < 100; ++i)
    t.tick();

  Action* a5 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 0);
  BOOST_REQUIRE(t.insert_action(a5, 14) == true);

  Action* a6 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 0);
  BOOST_REQUIRE(t.insert_action(a6, 4) == false);

  Action* a7 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 0);
  BOOST_REQUIRE(t.insert_action(a7, 14) == true);

  Action* a8 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 0);
  BOOST_REQUIRE(t.insert_action(a8, 14) == true);

  Action* a9 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 0);
  BOOST_REQUIRE(t.insert_action(a9, 15) == true);

  for (int i = 0; i < 100; ++i)
    t.tick();

  BOOST_REQUIRE(global_a == 7);
}

BOOST_AUTO_TEST_CASE(turn_handler_2)
{
  global_a = 0;

  TurnHandler t;

  Action* a = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 2);
  BOOST_REQUIRE(t.insert_action(a, 1) == true);
  Action* a2 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 2);
  BOOST_REQUIRE(t.insert_action(a2, 2) == true);

  // the action is not yet validated.
  for (int i = 0; i < 1; ++i)
    t.tick();
  BOOST_REQUIRE(global_a == 0);

  a->validate(1);
  a->validate(2);
  a2->validate(2);
  for (int i = 0; i < 200; ++i)
    t.tick();
  BOOST_REQUIRE(global_a == 1);

  a2->validate(1);
  for (int i = 0; i < 200; ++i)
    t.tick();
  BOOST_REQUIRE(global_a == 2);
}

BOOST_AUTO_TEST_CASE(turn_handler_3)
{
  TurnHandler t;

  Turn* turn;
  turn = t.get_turn(1);
  BOOST_REQUIRE(turn == 0);

  Action* a = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 2);
  BOOST_REQUIRE(t.insert_action(a, 1) == true);
  Action* a2 = new Action(std::bind(&coucou, std::placeholders::_1), new Event, 2);
  BOOST_REQUIRE(t.insert_action(a2, 0) == false);

  turn = t.get_turn(1);
  BOOST_REQUIRE(turn != 0);
  turn = t.get_turn(2);
  BOOST_REQUIRE(turn == 0);
}

BOOST_AUTO_TEST_SUITE_END()
