#include <gui/sprites/archive_sprite.hpp>
#include <world/building.hpp>
#include <gui/common.hpp>
#include <gui/screen/screen.hpp>
#include <world/world.hpp>
#include <logging/logging.hpp>
#include <gui/camera/camera.hpp>
#include <game/game_client.hpp>

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

void ArchiveSprite::draw(GameClient* game) const
{
  if (!this->is_in_screen(game->get_camera()))
    return;
  if (game->is_entity_selected(this->building) == true)
    this->draw_selection_circle(game->get_camera());

  Position pos(this->building->x * CELL_SIZE, this->building->y * CELL_SIZE);
  auto entpos = game->get_camera().world_to_camera_position(pos);
  sf::Sprite sprite = this->get_cursor_sprite();
  sprite.setPosition(entpos.x - game->get_camera().x, entpos.y - game->get_camera().y - LAYER_HEIGHT);
  game->get_camera().draw(sprite);

  if (game->is_entity_selected(this->building) == true)
    this->draw_rally_point(game->get_camera());
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
