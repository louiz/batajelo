#ifndef EMP_SPRITE_HPP_INCLUDED
#define EMP_SPRITE_HPP_INCLUDED

#include <gui/sprites/entity_sprite.hpp>

class EmpSprite: public EntitySprite
{
public:
  EmpSprite(const Entity* const);
  ~EmpSprite() = default;
  void draw(sf::RenderTarget& surface, const sf::RenderStates& states) const override final;
  void tick() override final;

private:
  mutable sf::CircleShape circle;

  EmpSprite(const EmpSprite&) = delete;
  EmpSprite(EmpSprite&&) = delete;
  EmpSprite& operator=(const EmpSprite&) = delete;
  EmpSprite& operator=(EmpSprite&&) = delete;
};

#endif /* EMP_SPRITE_HPP_INCLUDED */
