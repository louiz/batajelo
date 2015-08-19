#ifndef MOBILITY_SPRITES_HPP_INCLUDED
#define MOBILITY_SPRITES_HPP_INCLUDED

/**
 * Gives a way to draw all the camera sprites regarding the mobility of
 * world entities.  For example the flags to represent the path
 * selected entity is following.
 */

#include <gui/sprites/simple_world_sprite.hpp>
#include <world/path.hpp>

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class MobilitySprites
{
public:
  MobilitySprites() = default;
  ~MobilitySprites() = default;
  static void init();
  static void insert_flag_sprites(const Path& path,
                                  std::vector<std::unique_ptr<SimpleWorldSprite>>& flags);

private:
  static sf::Texture texture;
  static sf::Sprite flag_sprite;

  MobilitySprites(const MobilitySprites&) = delete;
  MobilitySprites(MobilitySprites&&) = delete;
  MobilitySprites& operator=(const MobilitySprites&) = delete;
  MobilitySprites& operator=(MobilitySprites&&) = delete;
};

#endif /* MOBILITY_SPRITES_HPP_INCLUDED */
