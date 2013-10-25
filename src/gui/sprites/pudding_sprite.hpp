#ifndef __PUDDING_SPRITE_HPP
# define __PUDDING_SPRITE_HPP

#include <gui/sprites/unit_sprite.hpp>

class PuddingSprite: public UnitSprite
{
public:
  PuddingSprite(const Unit* const);
  ~PuddingSprite() {}
  virtual void draw(Camera*, const ClientWorld*, const Screen*) const;
  virtual void tick();

  static bool init;

private:
  PuddingSprite(const PuddingSprite&);
  PuddingSprite& operator=(const PuddingSprite&);
  static sf::Texture body_texture;
  float boinc;
  float boincing;
};

#endif // __PUDDING_SPRITE_HPP
