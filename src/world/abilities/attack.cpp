#include <world/abilities/attack.hpp>
#include <world/entity.hpp>
#include <world/position.hpp>
#include <logging/logging.hpp>

#include <world/works/attack_work.hpp>

template<>
const std::string NamedAbility<Attack>::name = "Attack";

template<>
const AbilityType NamedAbility<Attack>::ability_type = AbilityType::Attack;

Attack::Attack(const utils::Duration fs_duration, const utils::Duration bs_duration,
               const Fix16 range):
  ActiveAbility(TargetType::Both),
  frontswing_duration(utils::duration_to_ticks(fs_duration)),
  backswing_duration(utils::duration_to_ticks(bs_duration)),
  range(range)
{
}

void Attack::cast(Entity* entity, World*, const Position& pos, const bool queue)
{
  log_debug("Attacking with entity " << entity->get_id() << " until position " << pos);
  auto work = std::make_unique<AttackWork>(entity, pos, this->range);
  if (queue)
    entity->queue_work(std::move(work));
  else
    entity->set_work(std::move(work));
}

void Attack::cast(Entity* entity, World *, const std::shared_ptr<Entity>& target, const bool queue)
{
  log_debug("Attacking with entity " << entity->get_id() << " the target " << target->get_id());
  auto work = std::make_unique<AttackWork>(entity, target, this->range);
  if (queue)
    entity->queue_work(std::move(work));
  else
    entity->set_work(std::move(work));
}

std::size_t Attack::get_frontswing_duration() const
{
  return this->frontswing_duration;
}

std::size_t Attack::get_backswing_duration() const
{
  return this->backswing_duration;
}
