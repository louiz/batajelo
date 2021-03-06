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

Dash::Dash(const Fix16 max_distance, const Fix16 max_concentration):
  ActiveAbility(TargetType::Point),
  max_distance(max_distance),
  max_concentration(max_concentration)
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
      log_warning("Work is not a ConcentrateWork, doing nothing.");
      return;
    }
  // Otherwise, use its value for the DashWork
  auto concentrate_task = static_cast<ConcentrateTask*>(concentrate->get_task());
  if (!concentrate_task)
    return;
  auto concentrate_value = concentrate_task->value();
  log_debug("The concentrate task value is: " << concentrate_value);
  concentrate_value = std::min(concentrate_value, this->get_max_concentration());
  log_debug("capped to " << concentrate_value);

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
      health->add(-concentrate_value);
    };
  auto work = std::make_unique<DashWork>(world, entity, pos, 50, this->max_distance, 50,
                                         on_impact, nullptr);
  entity->set_work(std::move(work));
}

const Fix16 Dash::get_max_distance() const
{
  return this->max_distance;
}

const Fix16 Dash::get_max_concentration() const
{
  return this->max_concentration;
}
