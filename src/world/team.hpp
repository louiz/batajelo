#ifndef TEAM_COMPONENT_HPP_INCLUDED
#define TEAM_COMPONENT_HPP_INCLUDED

#include <cstdint>

#include <world/components.hpp>

class Team: public Component
{
public:
  static const ComponentType component_type = ComponentType::Team;
  using type = uint8_t;
  Team(const type number);
  Team();
  ~Team() = default;
  type get() const;
  void set(const type number);

  void tick(Entity* entity, World* world) override final;

private:
  type number;

  Team(const Team&) = delete;
  Team(Team&&) = delete;
  Team& operator=(const Team&) = delete;
  Team& operator=(Team&&) = delete;
};


#endif /* TEAM_COMPONENT_HPP_INCLUDED */
