#include <gui/sprites/tourbillon_sprite.hpp>
#include <logging/logging.hpp>
#include <gui/camera/camera.hpp>
#include <game/game_client.hpp>
#include <world/health.hpp>
#include <world/manapool.hpp>
#include <world/team.hpp>
#include <world/location.hpp>

TourbillonSprite::TourbillonSprite(const Entity* const entity):
  EntitySprite(entity)
{
  if (TourbillonSprite::init == false)
    {
      if (!TourbillonSprite::body_texture.loadFromFile(AssetsManager::full_name("images/units/tourbillon.png")))
        exit(1);
      TourbillonSprite::body_texture.setSmooth(true);
      TourbillonSprite::init = true;
    }
}

void TourbillonSprite::draw(sf::RenderTarget& surface, const sf::RenderStates& states) const
{
  sf::Sprite sprite(TourbillonSprite::body_texture);
  const sf::Vector2u size = TourbillonSprite::body_texture.getSize();
  sprite.setOrigin(size.x/2, size.y);
  surface.draw(sprite, states);
}

void TourbillonSprite::tick()
{
}

bool TourbillonSprite::init = false;
sf::Texture TourbillonSprite::body_texture;
