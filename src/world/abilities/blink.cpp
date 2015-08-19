#include <world/abilities/blink.hpp>
#include <world/works/blink_work.hpp>
#include <world/entity.hpp>

#include <logging/logging.hpp>

#include <memory>

using namespace std::chrono_literals;

template<>
const std::string NamedAbility<Blink>::name = "Blink";

template<>
const AbilityType NamedAbility<Blink>::ability_type = AbilityType::Blink;

Blink::Blink():
  ActiveAbility(TargetType::Point)
{
  log_debug("Creating a blink instance");
  this->cooldown.set_max(0s);
  log_debug(this->cooldown.get_max_in_ticks());
}

void Blink::cast(Entity* entity, World* world, const Position& position, const bool queue)
{
  // Check mana, cooldown etc etc
  log_debug("CASTING blink for entity" << entity->get_id() << " to pos " << position);
  auto work = std::make_unique<BlinkWork>(world, entity, position);
  if (queue)
    entity->queue_work(std::move(work));
  else
    entity->set_work(std::move(work));
  this->cooldown.start();
}
