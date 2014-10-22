#ifndef __ARCHIVE_SPRITE_HPP__
# define __ARCHIVE_SPRITE_HPP__

#include <gui/sprites/building_sprite.hpp>

class ArchiveSprite: public BuildingSprite
{
public:
  ArchiveSprite(const Building* const building);
  ~ArchiveSprite();
  void draw(GameClient* game) const override final;
  void tick() override final;
  const sf::Sprite& get_cursor_sprite() const;
private:
  ArchiveSprite(const ArchiveSprite&);
  ArchiveSprite& operator=(const ArchiveSprite&);
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Color color;
};

#endif // __ARCHIVE_SPRITE_HPP__
