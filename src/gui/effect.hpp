#ifndef EFFECT_HPP_INCLUDED
#define EFFECT_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <gui/positionnable.hpp>
#include <utils/time.hpp>
#include <gui/sprites/world_sprite.hpp>

/**
 * Defines a graphical effect that lives on the Camera. It can contains
 * particles, light effects, etc.
 */

class Effect: public Positionnable, public WorldSprite
{
public:
  Effect(const Position& pos, const utils::Duration& lifetime):
    Positionnable(pos),
    lifetime(lifetime),
    life(lifetime)
  {
  }
  ~Effect() = default;
  virtual void update(const utils::Duration& dt)
  {
    this->life -= dt;
    this->on_update(dt);
  }
  const utils::Duration& current_life() const
  {
    return this->life;
  }
  Position get_world_pos() const override final
  {
    return this->position;
  }

private:
  virtual void on_update(const utils::Duration& dt) {}
  /**
   * Maximum duration of the effect.
   */
  const utils::Duration lifetime;
  /**
   * Current remaining life. Equals lifetime at the beginning. When it
   * reaches 0, the effect is destroyed.
   */
  utils::Duration life;

  Effect(const Effect&) = delete;
  Effect(Effect&&) = delete;
  Effect& operator=(const Effect&) = delete;
  Effect& operator=(Effect&&) = delete;
};

#endif /* EFFECT_HPP_INCLUDED */
