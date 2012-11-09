#include <gui/sprites/building_sprite.hpp>

void BuildingSprite::draw_rally_point(Camera*, const Building* const building)
{
}

bool BuildingSprite::is_in_screen(Camera* camera, const Building* const building) const
{
  const sf::Vector2u win_size = camera->get_win_size();
  Position pos(building->x * CELL_SIZE, building->y * CELL_SIZE);
  sf::Vector2u entpos = camera->world_to_camera_position(pos);
  if (((entpos.x > camera->x) && (entpos.x < camera->x + win_size.x) &&
       (entpos.y > camera->y) && (entpos.y < camera->y + win_size.y)))
    return true;
  return false;
}

void BuildingSprite::draw_selection_circle(Camera* camera, const Building* const building) const
{
  Position pos(building->x * CELL_SIZE + CELL_SIZE/2, building->y * CELL_SIZE + CELL_SIZE/2);
  sf::Vector2u entpos = camera->world_to_camera_position(pos);
  const unsigned short width = 65;

  // sf::Vector2f center(entpos.x - width/2 - camera->x, entpos.y - width/2 - camera->y);
  sf::Vector2f center(entpos.x - camera->x, entpos.y - camera->y);

  sf::CircleShape circle;
  circle.setRadius(width);
  circle.setOrigin(width, width);
  circle.setOutlineColor(sf::Color(0xff, 0xcb, 0xF6, 255));
  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineThickness(2);
  circle.setPosition(center);
  circle.setScale(sf::Vector2f(1, 3.f/4.f));
  camera->draw(circle);
}
