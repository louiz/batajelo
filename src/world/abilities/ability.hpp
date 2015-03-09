#ifndef ABILITY_HPP_INCLUDED
#define ABILITY_HPP_INCLUDED

#include <memory>
#include <string>

class World;
class Entity;
class Vec2;

using Position = Vec2;

enum class AbilityType
{
  Concentrate,
  Attack,
  Blink,
  Phase,
  Dash,
  Emp,

  count
};

class Ability
{
public:
  Ability() = default;
  virtual ~Ability() = default;
  virtual void tick(Entity*, World*) {}
  virtual void cast(Entity*, World*, const bool) {}
  virtual void cast(Entity*, World*, const Position&, const bool) {}
  virtual void cast(Entity*, World*, const std::shared_ptr<Entity>&, const bool) {}

  virtual const std::string& get_name() const = 0;
  virtual AbilityType get_type() const = 0;

private:
  Ability(const Ability&) = delete;
  Ability(Ability&&) = delete;
  Ability& operator=(const Ability&) = delete;
  Ability& operator=(Ability&&) = delete;
};

#endif /* ABILITY_HPP_INCLUDED */
