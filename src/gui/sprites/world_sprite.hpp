#ifndef __SPRITE_HPP__
# define __SPRITE_HPP__

#include <SFML/Graphics.hpp>

#include <world/position.hpp>
#include <gui/sprites/energy_bar.hpp>
#include <utils/time.hpp>
#include <assets/assets_manager.hpp>

class WorldSprite
{
public:
  WorldSprite() {}
  virtual ~WorldSprite() {}
  virtual void draw(sf::RenderTarget& surface, const sf::RenderStates& states) const = 0;
  // TODO remove this tick()
  virtual void tick() {};
  virtual void update(const utils::Duration& dt) {}
  virtual Position get_world_pos() const = 0;

  static const EnergyBar standard_health_bar;
  static const EnergyBar standard_mana_bar;

private:
  WorldSprite(const WorldSprite&);
  WorldSprite& operator=(const WorldSprite&);
};

#endif // __SPRITE_HPP__
