#include <gui/sprites/orange_blob.hpp>
#include <world/mobility.hpp>
#include <world/entity.hpp>

OrientedSpriteSheets OrangeBlob::idle_sheets;
OrientedSpriteSheets OrangeBlob::walk_sheets;

OrangeBlob::OrangeBlob(const Entity* const entity):
  EntitySprite(entity),
  idle_animation(OrangeBlob::idle_sheets.get_nb_textures(),
                 48ms),
  walk_animation(OrangeBlob::walk_sheets.get_nb_textures(),
                 50ms),
  current_animation(&idle_animation)
{
}

void OrangeBlob::init()
{
  OrangeBlob::idle_sheets.load(AssetsManager::full_name("images/char1/"), "idle", {128, 128});
  OrangeBlob::walk_sheets.load(AssetsManager::full_name("images/char1/"), "walk", {128, 128});
}

void OrangeBlob::draw(sf::RenderTarget& surface, const sf::RenderStates& states) const
{
  // this->draw_shadow(surface, states);
  auto frame = this->current_animation->get_current_frame();
  const OrientedSpriteSheets* sprite_sheets;
  switch (this->task_type)
    {
    case TaskType::Path:
    case TaskType::Follow:
      sprite_sheets = &OrangeBlob::walk_sheets;
      break;
    default:
      sprite_sheets = &OrangeBlob::idle_sheets;
      break;
    }
  Mobility* mobility = this->entity->get<Mobility>();
  auto direction = mobility->get_direction();
  // Get sprite_sheet depending on direction
  const auto& texture = sprite_sheets->get_texture(direction, frame);
  sf::Sprite sprite(texture);
  if (direction ==  Direction::E ||
      direction == Direction::SE ||
      direction == Direction::NE)
    sprite.scale(-1, 1);
  auto size = texture.getSize();
  sprite.setOrigin(size.x/2, size.y - 28);
  surface.draw(sprite, states);
}

void OrangeBlob::update(const utils::Duration& dt)
{
  this->current_animation->update(dt);
}

void OrangeBlob::on_task_changed(const Task* task)
{
  switch (this->task_type)
    {
    case TaskType::Path:
    case TaskType::Follow:
      this->current_animation = &this->walk_animation;
      break;
    default:
      this->current_animation = &this->idle_animation;
      break;
    }
  this->current_animation->reset();
}
