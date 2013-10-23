/** @addtogroup Gui
 *  @{
 */

/**
 * Defines a abstract class that we inherite to define how a specific kind
 * of building should be drawn on the screen.  The class receives the
 * Building object as a parameter, it gets various informations from it (its
 * position in the world, its current action (walking, attacking, etc) and
 * its current tick number in that action), from that it draws one or more
 * sf::Sprite or any other Drawable on the screen at various position.
 *
 * @class BuildingSprite
 */

#ifndef __BUILDING_SPRITE_HPP__
# define __BUILDING_SPRITE_HPP__

#include <gui/sprites/sprite.hpp>

class Building;
class ClientWorld;
class Screen;

class BuildingSprite: public Sprite
{
public:
  BuildingSprite(const Building*const building):
    building(building)
  {}
  virtual ~BuildingSprite() {};
  virtual void draw(Camera*, const ClientWorld*, const Screen*) const = 0;
  virtual void tick() = 0;
  /**
   * Returns the sprite that should be drawn on the cell under the cursor we
   * the left click action is to build this building.
   */
  virtual const sf::Sprite& get_cursor_sprite() const = 0;
  /**
   * Draws the rally point of the building. Called only by buildings
   * actually using a rally point.
   */
  void draw_rally_point(Camera*, const Building* const) const;
  void draw_selection_circle(Camera* camera, const Building* const building) const;
  bool is_in_screen(Camera*, const Building* const) const;
protected:
  const Building *const building;
private:
  BuildingSprite(const BuildingSprite&);
  BuildingSprite& operator=(const BuildingSprite&);
};

#endif // __BUILDING_SPRITE_HPP__
