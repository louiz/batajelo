#ifndef __PICPIC_SPRITE_HPP__
# define __PICPIC_SPRITE_HPP__

#include <gui/sprites/unit_sprite.hpp>

class PicpicSprite: public UnitSprite
{
public:
  PicpicSprite(const Unit* const);
  virtual ~PicpicSprite() {}
  virtual void draw(Camera*, const ClientWorld*, const Screen*) const;
  virtual void tick();
  static bool init;

private:
  PicpicSprite(const PicpicSprite&);
  PicpicSprite& operator=(const PicpicSprite&);
  static sf::Texture body_texture;
  static sf::Texture eye_texture;
  float height;
  float float_direction;
};

#endif // __PICPIC_SPRITE_HPP__
