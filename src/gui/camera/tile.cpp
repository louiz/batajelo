#include <gui/camera/tile.hpp>
#include <logging/logging.hpp>

GraphTile::GraphTile(const sf::Texture& texture, const sf::IntRect& rect):
  sprite(texture, rect)
{
  log_debug("Creating graphtile: " << rect.left << " " << rect.top << " " << rect.width << " " << rect.height);
}

GraphTile::~GraphTile()
{
}
