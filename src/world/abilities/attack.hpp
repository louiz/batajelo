#ifndef ATTACK_HPP_INCLUDED
#define ATTACK_HPP_INCLUDED

#include <world/abilities/active_ability.hpp>

class Attack: public ActiveAbility<Attack>
{
public:
  Attack(const std::size_t fs, const std::size_t bs);
  ~Attack() = default;
  void cast(Entity* entity, World *, const Position& pos, const bool queue) override final;
  void cast(Entity* entity, World *, const std::shared_ptr<Entity>& target, const bool queue) override final;

protected:
  std::size_t frontswing_duration;
  std::size_t backswing_duration;

private:
  Attack(const Attack&) = delete;
  Attack(Attack&&) = delete;
  Attack& operator=(const Attack&) = delete;
  Attack& operator=(Attack&&) = delete;
};

#endif /* ATTACK_HPP_INCLUDED */
