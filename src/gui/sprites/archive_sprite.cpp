#include <gui/sprites/archive_sprite.hpp>
#include <world/building.hpp>
#include <gui/common.hpp>
#include <gui/screen/screen.hpp>

ArchiveSprite::ArchiveSprite()
{
  if (!this->texture.loadFromFile("data/images/buildings/archive.png"))
    throw GraphInitError();
  this->sprite.setTexture(this->texture);
}

ArchiveSprite::~ArchiveSprite()
{
}

void ArchiveSprite::draw(Camera* camera, const ClientWorld* world, const Screen* screen, const Building* const building) const
{
  if (!this->is_in_screen(camera, building))
    return;
  if (world->is_entity_selected(building) == true)
    this->draw_selection_circle(camera, building);

  Position pos(building->x * CELL_SIZE, building->y * CELL_SIZE);
  sf::Vector2u entpos = camera->world_to_camera_position(pos);
  sf::Sprite sprite = this->get_cursor_sprite();
  sprite.setPosition(entpos.x - camera->x, entpos.y - camera->y - LAYER_HEIGHT);
  camera->draw(sprite);

  if (world->is_entity_selected(building) == true)
    this->draw_rally_point(camera, building);
}

const sf::Sprite& ArchiveSprite::get_cursor_sprite() const
{
  return this->sprite;
}
