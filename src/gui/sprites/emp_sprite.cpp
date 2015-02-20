#include <gui/sprites/emp_sprite.hpp>
#include <world/location.hpp>
#include <world/entity.hpp>
#include <game/game_client.hpp>

constexpr unsigned int emp_radius = 300;

EmpSprite::EmpSprite(const Entity* const entity):
  EntitySprite(entity),
  circle(emp_radius)
{
  this->circle.setOrigin(emp_radius, emp_radius);
  this->circle.setScale(1, 0.666f);
  this->circle.setFillColor({255, 0, 255, 123});
}

void EmpSprite::draw(GameClient* game) const
{
  Location* location = this->entity->get<Location>();
  assert(location);
  const auto entpos = game->get_camera().world_to_camera_position(location->position());
  const float x = entpos.x - game->get_camera().x;
  const float y = entpos.y - game->get_camera().y;

  this->circle.setPosition(x, y);
  game->get_camera().draw(this->circle);
}

void EmpSprite::tick()
{
}
