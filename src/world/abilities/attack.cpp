#include <world/abilities/attack.hpp>
#include <world/entity.hpp>
#include <world/position.hpp>
#include <logging/logging.hpp>

#include <world/works/attack_work.hpp>

template<>
const std::string NamedAbility<Attack>::name = "Attack";

template<>
const AbilityType NamedAbility<Attack>::ability_type = AbilityType::Attack;

Attack::Attack(const std::size_t fs, const std::size_t bs):
  ActiveAbility(TargetType::Both),
  frontswing_duration(fs),
  backswing_duration(bs)
{
}

void Attack::cast(Entity* entity, const Position& pos, const bool queue)
{
  log_debug("Attacking with entity " << entity->get_id() << " until position " << pos);
  auto work = std::make_unique<AttackWork>(entity, pos);
  if (queue)
    entity->queue_work(std::move(work));
  else
    entity->set_work(std::move(work));
}

void Attack::cast(Entity* entity, Entity* target, const bool queue)
{
}
