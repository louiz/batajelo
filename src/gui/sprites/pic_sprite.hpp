#ifndef __PICPIC_SPRITE_HPP__
# define __PICPIC_SPRITE_HPP__

#include <gui/sprites/entity_sprite.hpp>

class PicpicSprite: public EntitySprite
{
public:
  PicpicSprite(const Entity* const);

  void draw(GameClient* game) const override final;
  void tick() override final;
  static bool init;

private:
  float height;
  float float_direction;

  PicpicSprite(const PicpicSprite&);
  PicpicSprite& operator=(const PicpicSprite&);
  static sf::Texture body_texture;
  static sf::Texture eye_texture;
};

#endif // __PICPIC_SPRITE_HPP__
