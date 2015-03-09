#include <world/abilities/dash.hpp>
#include <world/entity.hpp>
#include <world/works/concentrate_work.hpp>
#include <world/works/dash_work.hpp>

#include <logging/logging.hpp>

template <>
const std::string NamedAbility<Dash>::name = "Dash";

template <>
const AbilityType NamedAbility<Dash>::ability_type = AbilityType::Dash;

Dash::Dash():
  ActiveAbility(TargetType::Point)
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
  auto value = concentrate_task->value();
  log_debug("The concentrate task value is: " << value);
  auto work = std::make_unique<DashWork>(entity, world, pos, 35,
                                         nullptr, nullptr);
  entity->set_work(std::move(work));
}
