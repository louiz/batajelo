#include <gui/sprites/entity_sprite.hpp>
#include <world/entity.hpp>

#include <world/location.hpp>

sf::Texture EntitySprite::shadow_texture;
bool EntitySprite::init = false;
const std::vector<sf::Color> EntitySprite::team_colors = {sf::Color::White,
                                                        sf::Color::Red,
                                                        sf::Color::Blue};

EntitySprite::EntitySprite(const Entity* const entity):
  entity(entity)
{
  if (EntitySprite::init == false)
    {
      if (!EntitySprite::shadow_texture.loadFromFile("data/images/units/shadow.png"))
        exit(1);
      EntitySprite::init = true;
    }
}

void EntitySprite::draw_shadow(Camera& camera, const sf::Color color) const
{
  Location* location = this->entity->get<Location>();
  assert(location);
  const auto entpos = camera.world_to_camera_position(location->position());
  const int x = entpos.x - camera.x;
  const int y = entpos.y - camera.y;
  const sf::Vector2u size = EntitySprite::shadow_texture.getSize();
  sf::Sprite sprite(EntitySprite::shadow_texture);
  sprite.setPosition(x - size.x/2, y - size.y/2);

  sprite.setColor(color);
  camera.draw(sprite);
}

const Entity* EntitySprite::get_entity() const
{
  return this->entity;
}

Position EntitySprite::get_world_pos() const
{
  Location* location = this->entity->get<Location>();
  assert(location);
  return location->position();
}
