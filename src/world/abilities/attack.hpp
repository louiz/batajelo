#ifndef ATTACK_HPP_INCLUDED
#define ATTACK_HPP_INCLUDED

#include <world/abilities/active_ability.hpp>
#include <utils/time.hpp>
#include <fixmath/fix16.hpp>

class Attack: public ActiveAbility<Attack>
{
public:
  Attack(const utils::Duration fs_duration, const utils::Duration bs_duration, const Fix16 range);
  ~Attack() = default;
  void cast(Entity* entity, World *, const Position& pos, const bool queue) override final;
  void cast(Entity* entity, World *, const std::shared_ptr<Entity>& target, const bool queue) override final;

  std::size_t get_frontswing_duration() const;
  std::size_t get_backswing_duration() const;

private:
  std::size_t frontswing_duration;
  std::size_t backswing_duration;
  Fix16 range;

  Attack(const Attack&) = delete;
  Attack(Attack&&) = delete;
  Attack& operator=(const Attack&) = delete;
  Attack& operator=(Attack&&) = delete;
};

#endif /* ATTACK_HPP_INCLUDED */
