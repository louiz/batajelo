#include <gui/sprites/bullet_sprite.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/entity.hpp>

#include <gui/effects/smoke_trail.hpp>

bool BulletSprite::init = false;
sf::Texture BulletSprite::texture;

BulletSprite::BulletSprite(const Entity* const entity):
  EntitySprite(entity)
{
  if (this->init == false)
    {
      if (!this->texture.loadFromFile(AssetsManager::full_name("images/bullet.png")))
        exit(1);
      this->texture.setSmooth(true);
      this->init = true;
    }
}

void BulletSprite::draw(sf::RenderTarget& surface, const sf::RenderStates& states) const
{
  sf::Sprite sprite(this->texture);
  sprite.setOrigin(this->texture.getSize().x/2, this->texture.getSize().y/2);
  sprite.setScale({0.4, 0.4});
  Mobility* mobility = this->entity->get<Mobility>();
  assert(mobility);
  sprite.setRotation(mobility->get_angle().to_double());

  surface.draw(sprite, states);
}

void BulletSprite::tick()
{
}
