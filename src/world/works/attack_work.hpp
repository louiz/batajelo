#ifndef ATTACK_WORK_INCLUDED_HPP
#define ATTACK_WORK_INCLUDED_HPP

#include <world/work.hpp>
#include <world/position.hpp>
#include <world/location.hpp>

#include <memory>

class Entity;

/**
 * Move until the destination is reached
 */

class AttackWork: public Work
{
public:
  AttackWork(Entity* entity, const Position& destination);
  AttackWork(Entity* entity, std::weak_ptr<Entity> target,
             const Position& destination = Position::invalid);
  ~AttackWork() = default;
  bool tick(World* world) override final;

private:
  void try_acquire_target(World* world);

  // The entity position, to do the acquire-target thing
  Location* location;
  Fix16 acquire_distance;
  /**
   * Only used when attacking a position, if we attack a target, it's set to
   * Position::invalid.
   */
  const Position destination;
  /**
   * Only used when attacking a specific target
   */
  const std::weak_ptr<Entity> target;

  AttackWork(const AttackWork&) = delete;
  AttackWork(AttackWork&&) = delete;
  AttackWork& operator=(const AttackWork&) = delete;
  AttackWork& operator=(AttackWork&&) = delete;
};

#endif /* ATTACK_WORK_INCLUDED_HPP */
