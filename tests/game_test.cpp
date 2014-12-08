#include <game/turn_handler.hpp>

#include <gtest/gtest.h>

TEST(TurnHandler, Creation)
{
  TurnHandler t;
  ASSERT_EQ(t.get_current_turn(), 0);
}

TEST(Turn, Move)
{
  Turn t;
  Turn t2 = std::move(t);
  ASSERT_EQ(t2.is_ready(), false);
  t2.mark_ready();
  t = std::move(t);
  ASSERT_EQ(t2.is_ready(), true);
}

TEST(TurnHandler, BasicAdvancement)
{
  TurnHandler t;
  ASSERT_EQ(t.get_current_turn(), 0);

  // The turn 1 is not ready, so even if we tick enough, we never go after
  // the turn 0
  for (auto i = 0u; i < TURN_TICKS; ++i)
    t.tick();
  ASSERT_EQ(0, t.get_current_turn());
  for (auto i = 0u; i < TURN_TICKS; ++i)
    t.tick();
  ASSERT_EQ(0, t.get_current_turn());

  t.mark_turn_as_ready();
  // Now the turn is ready, we can advance to the next turn just with one
  // tick
  t.tick();
  ASSERT_EQ(1, t.get_current_turn());

  // Advance 4 turn (but fail because turn 2 is not ready)
  for (auto i = 0u; i < TURN_TICKS*4; ++i)
    t.tick();
  ASSERT_EQ(1, t.get_current_turn());

  // Mark turns up to turn 5 as ready
  t.mark_turn_as_ready();
  t.mark_turn_as_ready();
  t.mark_turn_as_ready();
  t.mark_turn_as_ready();

  // Advance to turn 2
  t.tick();
  ASSERT_EQ(2, t.get_current_turn());

  // Advance one turn and a little bit more
  for (auto i = 0u; i < TURN_TICKS + 2; ++i)
    t.tick();
  ASSERT_EQ(3, t.get_current_turn());
}

TEST(TurnHandler, ExecuteActionOnTurnAdvancement)
{
  auto val = 0;

  TurnHandler t;
  ASSERT_EQ(t.get_current_turn(), 0);

  // Insert an action to be executed on turn 2
  t.insert_action([&val](){ val++; }, 2);

  // Insert two actions to be executed on turn 3
  t.insert_action([&val](){ val++; }, 3);
  t.insert_action([&val](){ val++; }, 3);

  // Mark turns up to turn 5 as ready
  for (auto i = 0u; i < 5; ++i)
    t.mark_turn_as_ready();

  ASSERT_EQ(0, val);
  // Go to turn 1, does not execute any action
  for (auto i = 0u; i < TURN_TICKS; ++i)
    t.tick();
  ASSERT_EQ(0, val);

  // Action on turn 2 is executed
  for (auto i = 0u; i < TURN_TICKS; ++i)
    t.tick();
  ASSERT_EQ(1, val);

  for (auto i = 0u; i < TURN_TICKS; ++i)
    t.tick();
  ASSERT_EQ(3, val);

}

TEST(TurnHandler, ActionCannotBeAddedInReadyTurn)
{
  TurnHandler t;
  // mark turn 1 as ready
  t.mark_turn_as_ready();
  // Try to insert an action in turn 1
  ASSERT_DEATH(t.insert_action([](){}, 1), "");
}

TEST(TurnHandler, NextTurnCallback)
{
  TurnHandler t;
  auto turn = t.get_current_turn();
  ASSERT_EQ(0, turn);
  t.set_next_turn_callback([&turn](const TurnNb nb) { turn = nb; });
  t.mark_turn_as_ready();
  t.mark_turn_as_ready();
  for (auto i = 0u; i < TURN_TICKS*2; ++i)
    t.tick();
  ASSERT_EQ(2, turn);
}
