#ifndef __ARCHIVE_SPRITE_HPP__
# define __ARCHIVE_SPRITE_HPP__

#include <gui/sprites/building_sprite.hpp>

class ArchiveSprite: public BuildingSprite
{
public:
  ArchiveSprite(const Building* const building);
  ~ArchiveSprite();
  void draw(Camera*, const ClientWorld*, const Screen*) const;
  void tick();
  const sf::Sprite& get_cursor_sprite() const;
private:
  ArchiveSprite(const ArchiveSprite&);
  ArchiveSprite& operator=(const ArchiveSprite&);
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Color color;
};

#endif // __ARCHIVE_SPRITE_HPP__
