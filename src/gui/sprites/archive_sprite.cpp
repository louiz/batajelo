#include <gui/sprites/archive_sprite.hpp>
#include <world/building.hpp>
#include <gui/common.hpp>
#include <gui/screen/screen.hpp>
#include <world/client_world/client_world.hpp>
#include <logging/logging.hpp>

ArchiveSprite::ArchiveSprite(const Building* const building):
  BuildingSprite(building)
{
  if (!this->texture.loadFromFile("data/images/buildings/archive.png"))
    throw GraphInitError();
  this->sprite.setTexture(this->texture);
}

ArchiveSprite::~ArchiveSprite()
{
}

void ArchiveSprite::draw(Camera* camera, const ClientWorld* world, const Screen* screen) const
{
  if (!this->is_in_screen(camera, this->building))
    return;
  if (world->is_entity_selected(this->building) == true)
    this->draw_selection_circle(camera, this->building);

  Position pos(this->building->x * CELL_SIZE, this->building->y * CELL_SIZE);
  sf::Vector2u entpos = camera->world_to_camera_position(pos);
  sf::Sprite sprite = this->get_cursor_sprite();
  sprite.setPosition(entpos.x - camera->x, entpos.y - camera->y - LAYER_HEIGHT);
  camera->draw(sprite);

  if (world->is_entity_selected(this->building) == true)
    this->draw_rally_point(camera, this->building);
}

const sf::Sprite& ArchiveSprite::get_cursor_sprite() const
{
  return this->sprite;
}

void ArchiveSprite::tick()
{
  if (this->building->works.empty())
    this->sprite.setColor(sf::Color::White);
  else
    {
      if (this->color.r < 255U)
        ++this->color.r;
      else if (this->color.g < 255U)
        ++this->color.g;
      else if (this->color.b < 255U)
        ++this->color.b;
      else
        this->color = sf::Color::Black;
      this->sprite.setColor(this->color);
    }
}
