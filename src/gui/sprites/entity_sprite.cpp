#include <gui/sprites/entity_sprite.hpp>
#include <world/entity.hpp>
#include <world/task.hpp>

#include <world/location.hpp>

#include <logging/logging.hpp>

sf::Texture EntitySprite::shadow_texture;
bool EntitySprite::init = false;
const std::vector<sf::Color> EntitySprite::team_colors = {sf::Color::White,
                                                        sf::Color::Red,
                                                        sf::Color::Blue};

EntitySprite::EntitySprite(const Entity* const entity):
  entity(entity),
  task_type(TaskType::None)
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

bool EntitySprite::is_mouse_over(const Camera* camera) const
{
  Location* location = this->entity->get<Location>();
  if (!location)
    return false;

  const auto pos = camera->get_mouse_position();
  Position mouse_pos = camera->camera_to_world_position(pos.x,
                                                        pos.y);
  Position ent_pos = location->position();

  // TODO, use different values depending on the sprite's size
  return (mouse_pos.x > ent_pos.x - 50 &&
          mouse_pos.x < ent_pos.x + 50 &&
          mouse_pos.y > ent_pos.y - 80 &&
          mouse_pos.y < ent_pos.y + 20);
}

void EntitySprite::set_task(const Task* task)
{
  log_debug("EntitySprite::set_task" << static_cast<int>(task->get_type()));

  this->task_type = task->get_type();
  this->on_task_changed(task);
}
