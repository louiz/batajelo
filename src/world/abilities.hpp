#ifndef ABILITIES_HPP_INCLUDED
#define ABILITIES_HPP_INCLUDED

/**
 * Container for one or more abilities.
 */

#include <world/components.hpp>
#include <world/abilities/ability.hpp>

#include <memory>
#include <vector>

#include <cstddef>

class Abilities: public Component
{
public:
  static const ComponentType component_type = ComponentType::Abilities;
  Abilities(const std::size_t size,
            const std::size_t fs, const std::size_t bs):
    abilities(size),
    frontswing(fs),
    backswing(bs)
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
  /**
   * Look for an Ability with that type.
   */
  Ability* find(const AbilityType& type) const;

  const std::size_t& frontswing_duration() const;
  const std::size_t& backswing_duration() const;

private:
  std::vector<std::unique_ptr<Ability>> abilities;
  /**
   * The duration, in ticks, of the two phases of a casted ability
   */
  const std::size_t frontswing;
  const std::size_t backswing;

  Abilities(const Abilities&) = delete;
  Abilities(Abilities&&) = delete;
  Abilities& operator=(const Abilities&) = delete;
  Abilities& operator=(Abilities&&) = delete;
};

template <typename T>
T* get_ability(Entity* entity, const AbilityType& type);

Ability* get_ability(Entity* entity, const AbilityType& type);
#endif /* ABILITIES_HPP_INCLUDED */