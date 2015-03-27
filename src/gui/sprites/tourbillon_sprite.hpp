#ifndef TOURBILLON_SPRITE_HPP_INCLUDED
#define TOURBILLON_SPRITE_HPP_INCLUDED

#include <gui/sprites/entity_sprite.hpp>

class TourbillonSprite: public EntitySprite
{
public:
  TourbillonSprite(const Entity* const);
  void draw(GameClient* game) const override final;
  void tick() override final;
  ~TourbillonSprite() = default;

private:
  static bool init;
  static sf::Texture body_texture;

  TourbillonSprite(const TourbillonSprite&) = delete;
  TourbillonSprite(TourbillonSprite&&) = delete;
  TourbillonSprite& operator=(const TourbillonSprite&) = delete;
  TourbillonSprite& operator=(TourbillonSprite&&) = delete;
};

#endif /* TOURBILLON_SPRITE_HPP_INCLUDED */
