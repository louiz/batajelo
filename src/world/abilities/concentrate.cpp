#include <world/abilities/concentrate.hpp>
#include <world/works/concentrate_work.hpp>
#include <world/entity.hpp>

#include <logging/logging.hpp>

template <>
const std::string NamedAbility<Concentrate>::name = "Concentrate";

template <>
const AbilityType NamedAbility<Concentrate>::ability_type = AbilityType::Concentrate;

Concentrate::Concentrate():
  ActiveAbility(TargetType::None)
{
}

void Concentrate::cast(Entity* entity, World* world, const bool queue)
{
  log_debug("Concentrate::cast. Starting concentrate task");
  // TODO queue, or disable queue alltogether
  auto work = std::make_unique<ConcentrateWork>(entity, world);
  entity->set_work(std::move(work));
}
