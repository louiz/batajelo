#include <world/abilities/dash.hpp>
#include <world/entity.hpp>
#include <world/works/concentrate_work.hpp>
#include <world/works/dash_work.hpp>
#include <world/world.hpp>
#include <world/world_callbacks.hpp>
#include <world/team.hpp>
#include <world/health.hpp>

#include <logging/logging.hpp>

template <>
const std::string NamedAbility<Dash>::name = "Dash";

template <>
const AbilityType NamedAbility<Dash>::ability_type = AbilityType::Dash;

Dash::Dash(const Fix16 max_distance):
  ActiveAbility(TargetType::Point),
  max_distance(max_distance)
{
}

void Dash::cast(Entity* entity, World* world, const Position& pos, const bool queue)
{
  log_debug("Dash::cast. Starting DashWork");
  // Get the ConcentrateWork of the entity
  auto concentrate = dynamic_cast<ConcentrateWork*>(entity->get_current_work());
  if (concentrate == nullptr)
    {
      // If none is found, that's weird, but do nothing
      log_debug("Work is not a ConcentrateWork, doing nothing.");
      return;
    }
  // Otherwise, use its value for the DashWork
  Task* task = concentrate->get_task();
  if (!task)
    return;
  auto concentrate_task = static_cast<ConcentrateTask*>(task);
  auto concentrate_value = concentrate_task->value();
  log_debug("The concentrate task value is: " << concentrate_value);

  Team* own_team = entity->get<Team>();
  assert(own_team);

  auto on_impact = [entity, world, concentrate_value, own_team](Entity* impacted_entity)
    {
      Team* team = impacted_entity->get<Team>();
      // Do nothing on our teammates
      if (team->get() == own_team->get())
        return;
      Health* health = impacted_entity->get<Health>();
      if (!health)
        return;
      world->callbacks->impact(entity, impacted_entity);
      health->add(-concentrate_value/2);
    };
  auto work = std::make_unique<DashWork>(entity, world, pos, 35, this->max_distance, 50,
                                         on_impact, nullptr);
  entity->set_work(std::move(work));
}
