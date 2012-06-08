#include <gui/camera/tile.hpp>

GraphTile::GraphTile(const sf::Texture& texture, const sf::IntRect& rect):
  sprite(texture, rect)
{
}

GraphTile::~GraphTile()
{
}
