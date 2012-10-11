/** @addtogroup Gui
 *  @{
 */

/**
 * Defines a abstract class that we inherite to define how a specific kind
 * of building should be drawn on the screen.  The class receives the
 * Building object as a parameter, it gets various informations from it (its
 * position in the world, its current action (walking, attacking, etc) and
 * its current tick number in that action), from that it draws one or more
 * sf::Sprite on the screen at various position.
 *
 * @class BuildingSprite
 */

#ifndef __BUILDING_SPRITE_HPP__
# define __BUILDING_SPRITE_HPP__

#include <SFML/Graphics.hpp>

class Building;

class BuildingSprite
{
public:
  BuildingSprite() {};
  virtual ~BuildingSprite() {};
  virtual void draw(const Building*) const = 0;
  /**
   * Returns the sprite that should be drawn on the cell under the cursor we
   * the left click action is to build this building.
   */
  virtual sf::Sprite get_cursor_sprite() const = 0;
private:
  BuildingSprite(const BuildingSprite&);
  BuildingSprite& operator=(const BuildingSprite&);
};

#endif // __BUILDING_SPRITE_HPP__
