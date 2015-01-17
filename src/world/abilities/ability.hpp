#ifndef ABILITY_HPP_INCLUDED
#define ABILITY_HPP_INCLUDED

#include <string>

class World;
class Entity;

class Ability
{
public:
  Ability() = default;
  virtual ~Ability() = default;
  virtual void tick(Entity* entity, World* world) {}

  virtual const std::string& get_name() const = 0;

private:
  Ability(const Ability&) = delete;
  Ability(Ability&&) = delete;
  Ability& operator=(const Ability&) = delete;
  Ability& operator=(Ability&&) = delete;
};

#endif /* ABILITY_HPP_INCLUDED */
