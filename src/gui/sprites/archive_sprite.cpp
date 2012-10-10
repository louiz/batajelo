#include <gui/sprites/archive_sprite.hpp>
#include <world/building.hpp>
#include <gui/common.hpp>

ArchiveSprite::ArchiveSprite()
{
  if (!this->texture.loadFromFile("data/images/buildings/archive.png"))
    throw GraphInitError();
  this->sprite.setTexture(this->texture);
}

ArchiveSprite::~ArchiveSprite()
{
}

void ArchiveSprite::draw(const Building* building) const
{
}

sf::Sprite& ArchiveSprite::get_cursor_sprite()
{
  return this->sprite;
}
