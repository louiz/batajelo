#include <world/abilities/phase.hpp>
#include <world/world.hpp>
#include <world/world_callbacks.hpp>
#include <world/status/phase_status.hpp>

#include <world/entity.hpp>

#include <logging/logging.hpp>

template<>
const std::string NamedAbility<Phase>::name = "Phase boots";

template<>
const AbilityType NamedAbility<Phase>::ability_type = AbilityType::Phase;

Phase::Phase():
  ActiveAbility(TargetType::None)
{
}

void Phase::cast(Entity* entity, World* world, const Position&, const bool queue)
{
  log_debug("CASTING phase for entity " << entity->get_id());
  entity->add_status<PhaseStatus>(world);
  world->callbacks->ability_casted(nullptr, AbilityType::Phase,
                                  entity, Position::invalid);
}
