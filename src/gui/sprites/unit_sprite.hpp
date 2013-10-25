#ifndef __UNIT_SPRITE_HPP__
# define __UNIT_SPRITE_HPP__

#include <gui/sprites/sprite.hpp>
#include <world/unit.hpp>

class UnitSprite: public Sprite
{
public:
  UnitSprite(const Unit* const unit);
  ~UnitSprite() {}
  virtual void draw(Camera*, const ClientWorld*, const Screen*) const = 0;
protected:
  const Unit* const unit;
  void draw_shadow(Camera*, const ClientWorld*, const Screen*) const;
private:
  UnitSprite(const UnitSprite&);
  UnitSprite& operator=(const UnitSprite&);
  static sf::Texture shadow_texture;
  static bool init;
};

#endif // __UNIT_SPRITE_HPP__
