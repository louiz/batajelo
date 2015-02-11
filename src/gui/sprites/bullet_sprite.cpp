#include <gui/sprites/bullet_sprite.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/entity.hpp>
#include <game/game_client.hpp>

bool BulletSprite::init = false;
sf::Texture BulletSprite::texture;

BulletSprite::BulletSprite(const Entity* const entity):
  EntitySprite(entity)
{
  if (this->init == false)
    {
      if (!this->texture.loadFromFile("data/images/bullet.png"))
        exit(1);
      this->init = true;
    }
}

void BulletSprite::draw(GameClient* game) const
{
  sf::Sprite sprite(this->texture);
  sprite.setOrigin(32, 32);
  sprite.setScale({0.4, 0.4});
  Mobility* mobility = this->entity->get<Mobility>();
  assert(mobility);
  sprite.setRotation(mobility->get_angle().to_double());

  Location* location = this->entity->get<Location>();
  assert(location);
  const auto entpos = game->get_camera().world_to_camera_position(location->position());
  const float x = entpos.x - game->get_camera().x;
  const float y = entpos.y - game->get_camera().y;

  sprite.setPosition(x, y);

  game->get_camera().draw(sprite);
}

void BulletSprite::tick()
{
}
