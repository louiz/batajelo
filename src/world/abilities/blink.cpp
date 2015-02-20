#include <world/abilities/blink.hpp>
#include <world/works/blink_work.hpp>
#include <world/entity.hpp>

#include <logging/logging.hpp>

#include <memory>

template<>
const std::string NamedAbility<Blink>::name = "Blink";

template<>
const AbilityType NamedAbility<Blink>::ability_type = AbilityType::Blink;

Blink::Blink():
  ActiveAbility(TargetType::Point)
{
}

void Blink::cast(Entity* entity, World*, const Position& position, const bool queue)
{
  // Check mana, cooldown etc etc
  log_debug("CASTING blink for entity" << entity->get_id() << " to pos " << position);
  auto work = std::make_unique<BlinkWork>(entity, position);
  if (queue)
    entity->queue_work(std::move(work));
  else
    entity->set_work(std::move(work));
}
