/**
 * Base class for GameServer and GameClient
 */

#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <world/world.hpp>
#include <world/entity.hpp>

#include <utils/time.hpp>
#include <game/turn_handler.hpp>
#include <game/occupants_handler.hpp>

class Message;

class Game
{
public:
  Game();
  ~Game();

  void new_entity_callback(Message* msg);
  void move_callback(Message* msg);
  void cast_callback(Message* msg);
  utils::FloatingSeconds current_world_time() const;

protected:
  World world;
  OccupantsHandler occupants_handler;
  TurnHandler turn_handler;

private:
  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;
};


#endif /* GAME_HPP_INCLUDED */
