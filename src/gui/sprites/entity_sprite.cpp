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

void EntitySprite::draw_shadow(sf::RenderTarget& surface, const sf::RenderStates& states) const
{
  const sf::Vector2u size = EntitySprite::shadow_texture.getSize();
  sf::Sprite sprite(EntitySprite::shadow_texture);
  sprite.setOrigin(size.x/2, size.y/2);
  surface.draw(sprite, states);
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

void EntitySprite::set_task(const Task* task)
{
  log_debug("EntitySprite::set_task" << static_cast<int>(task->get_type()));

  this->task_type = task->get_type();
  this->on_task_changed(task);
}
