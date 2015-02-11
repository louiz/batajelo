#ifndef WORLD_CALLBACKS_HPP_INCLUDED
#define WORLD_CALLBACKS_HPP_INCLUDED

#include <functional>
#include <world/abilities/ability.hpp>
#include <world/position.hpp>
#include <world/entity.hpp>
#include <world/work.hpp>

/**
 * A list of callbacks that are called on various world events. For example
 * if a spell is cast, the World instance calls
 * WorldCallbacks::spell_casted() with the correct arguments.  This callback
 * may start an animation and/or sound on the player screen (if this is a
 * GameClient), or broadcast a message to some “web observers” for example
 * if this is a GameServer.
 *
 * We use the type std::function<> to be able to use std::bind, to set any
 * callback without any restriction. Also it is possible to set only some of
 * these callbacks and completely ignore the rest of them.
 */

class WorldCallbacks
{
public:
  WorldCallbacks() = default;
  ~WorldCallbacks() = default;

  /**
   * Called when an ability has been casted (the actual effect starts).
   */
  std::function<void(const Entity*, const AbilityType&, const Entity*, const Position&)> ability_casted;

private:
  WorldCallbacks(const WorldCallbacks&) = delete;
  WorldCallbacks(WorldCallbacks&&) = delete;
  WorldCallbacks& operator=(const WorldCallbacks&) = delete;
  WorldCallbacks& operator=(WorldCallbacks&&) = delete;
};


#endif /* WORLD_CALLBACKS_HPP_INCLUDED */
