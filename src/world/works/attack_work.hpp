#ifndef ATTACK_WORK_INCLUDED_HPP
#define ATTACK_WORK_INCLUDED_HPP

#include <world/work.hpp>
#include <world/position.hpp>

#include <memory>

class Entity;
class World;
class Location;
class Attack;

class AttackWork: public Work
{
public:
  AttackWork(World* world, Entity* entity, const Position& destination, const Fix16 range);
  AttackWork(World* world, Entity* entity, std::weak_ptr<Entity> target, const Fix16 range,
             const Position& destination = Position::invalid);
  ~AttackWork() = default;
  bool tick() override final;

private:
  void try_acquire_target(World* world);

  /**
   * The entity position, to do the acquire-target thing
   */
  Location* location;
  /**
   * Only used when attacking a position, if we attack a target, it's set to
   * Position::invalid.
   */
  const Position destination;
  /**
   * Only used when attacking a specific target
   */
  const std::weak_ptr<Entity> target;
  /**
   * Pointer on the ability of the attacking entity.  We need it to get the
   * attack's attributes of this entity (for example the backswing,
   * frontswing, damage, etc)
   */
  Attack* attack;
  /**
   * The range of the attack. If it's 0, it's a melee attack (we do not
   * spawn any projectile)
   */
  const Fix16 range;

  AttackWork(const AttackWork&) = delete;
  AttackWork(AttackWork&&) = delete;
  AttackWork& operator=(const AttackWork&) = delete;
  AttackWork& operator=(AttackWork&&) = delete;
};

#endif /* ATTACK_WORK_INCLUDED_HPP */
