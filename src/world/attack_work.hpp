#ifndef ATTACK_WORK_INCLUDED_HPP
#define ATTACK_WORK_INCLUDED_HPP

#include <world/path_work.hpp>
#include <world/work.hpp>
#include <world/position.hpp>
#include <world/location.hpp>

#include <memory>

class Entity;

class AttackWork: public Work
{
public:
  AttackWork(Entity* entity, const Position& position);
  ~AttackWork() = default;
  bool tick(World* world) override final;

private:
  /**
   */
  bool try_acquire_target(World* world);
  void attack_steps(World* world);
  void reset();

  PathWork path_work;
  Location* location;
  const Entity* entity;
  Fix16 acquire_distance;
  bool attack_point_reached;
  std::size_t frontswing;
  std::size_t backswing;
  std::weak_ptr<Entity> locked_target;

  AttackWork(const AttackWork&) = delete;
  AttackWork(AttackWork&&) = delete;
  AttackWork& operator=(const AttackWork&) = delete;
  AttackWork& operator=(AttackWork&&) = delete;
};

#endif /* ATTACK_WORK_INCLUDED_HPP */
