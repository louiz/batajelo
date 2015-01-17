#ifndef ABILITIES_HPP_INCLUDED
#define ABILITIES_HPP_INCLUDED

/**
 * Container for one or more abilities.
 * This is static
 */

#include <world/components.hpp>
#include <world/abilities/ability.hpp>

#include <memory>
#include <vector>

class Abilities: public Component
{
public:
  static const ComponentType component_type = ComponentType::Abilities;
  Abilities(const std::size_t size):
    abilities(size)
  {}
  ~Abilities() = default;
  void tick(Entity* entity, World* world) override final
  {
    for (const auto& ability: this->abilities)
      {
        if (ability)
          ability->tick(entity, world);
      }
  }
  void add(const std::size_t index, std::unique_ptr<Ability> ability);

  const std::vector<std::unique_ptr<Ability>>& get() const;
  const Ability* get(const std::size_t index) const;

private:
  std::vector<std::unique_ptr<Ability>> abilities;

  Abilities(const Abilities&) = delete;
  Abilities(Abilities&&) = delete;
  Abilities& operator=(const Abilities&) = delete;
  Abilities& operator=(Abilities&&) = delete;
};

#endif /* ABILITIES_HPP_INCLUDED */
