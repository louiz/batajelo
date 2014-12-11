#ifndef __UNIT_SPRITE_HPP__
# define __UNIT_SPRITE_HPP__

#include <gui/sprites/world_sprite.hpp>
#include <gui/camera/camera.hpp>

#include <vector>

class Unit;

class UnitSprite: public WorldSprite
{
public:
  UnitSprite(const Unit* const unit);
  ~UnitSprite() {}
  const Unit* const get_unit() const;
  Position get_world_pos() const override final;

protected:
  const Unit* const unit;
  void draw_shadow(Camera& camera, const sf::Color color) const;
  static const std::vector<sf::Color> team_colors;

private:
  static bool init;
  static sf::Texture shadow_texture;

  UnitSprite(const UnitSprite&);
  UnitSprite& operator=(const UnitSprite&);
};

#endif // __UNIT_SPRITE_HPP__
