/**
 * Base class for GameServer and GameClient
 */

#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <world/world.hpp>
#include <network/message.hpp>
#include <game/turn_handler.hpp>
#include <game/occupants_handler.hpp>

class Game
{
public:
  Game();
  ~Game();

  void new_unit_callback(Message* msg);
  void move_callback(Message* msg);
  virtual void do_new_unit(const EntityType type, const Position& pos,
                           const uint16_t team);

protected:
  World world;
  OccupantsHandler occupants_handler;
  TurnHandler turn_handler;

  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;
};


#endif /* GAME_HPP_INCLUDED */
