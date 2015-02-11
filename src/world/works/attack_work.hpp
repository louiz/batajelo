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
  ~AttackWork() = default;
  bool tick(World* world) override final;

private:
  void try_acquire_target(World* world);

  // The entity position, to do the acquire-target thing
  Location* location;
  Fix16 acquire_distance;
  const Position destination;

  AttackWork(const AttackWork&) = delete;
  AttackWork(AttackWork&&) = delete;
  AttackWork& operator=(const AttackWork&) = delete;
  AttackWork& operator=(AttackWork&&) = delete;
};

#endif /* ATTACK_WORK_INCLUDED_HPP */
