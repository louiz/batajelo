#include <world/abilities/emp.hpp>
#include <world/world.hpp>
#include <world/entity.hpp>
#include <world/team.hpp>
#include <world/works/single_task_work.hpp>
#include <world/tasks/emp.hpp>
#include <world/world_callbacks.hpp>

using EmpWork = SingleTaskWork<EmpTask>;

template<>
const std::string NamedAbility<Emp>::name = "EMP";

template<>
const AbilityType NamedAbility<Emp>::ability_type = AbilityType::Emp;

Emp::Emp():
  ActiveAbility(TargetType::Point)
{
}

void Emp::cast(Entity* entity, World* world, const Position& position, const bool queue)
{
  Team* team = entity->get<Team>();
  if (!team)
    return;

  auto emp = world->do_new_entity(2, position, team->get());
  emp->set_work(std::make_unique<EmpWork>(emp, world));
  world->callbacks->ability_casted(entity, AbilityType::Emp, nullptr, position);
}
