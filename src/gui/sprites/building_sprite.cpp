#include <logging/logging.hpp>
#include <gui/sprites/building_sprite.hpp>
#include <world/client_world/client_world.hpp>
#include <gui/camera/camera.hpp>

BuildingSprite::BuildingSprite(const Building* const building):
  building(building)
{
}

void BuildingSprite::draw_rally_point(Camera& camera) const
{
  if (this->building->rally_point == Vec2::zero)
    return ;
  const sf::Vector2u start_cam_pos = camera.world_to_camera_position(this->building->get_center_position());
  const sf::Vector2f start(start_cam_pos.x - camera.x,
                           start_cam_pos.y - camera.y);
  const sf::Vector2u end_cam_pos = camera.world_to_camera_position(this->building->rally_point);
  const sf::Vector2f end(end_cam_pos.x - camera.x,
                         end_cam_pos.y - camera.y);
  sf::VertexArray line(sf::Lines, 2);
  line[0].position = start;
  line[1].position = end;
  camera.draw(line);
}

bool BuildingSprite::is_in_screen(Camera& camera) const
{
  const sf::Vector2u win_size = camera.get_win_size();
  Position pos(this->building->x * CELL_SIZE, this->building->y * CELL_SIZE);
  sf::Vector2u entpos = camera.world_to_camera_position(pos);
  if (((entpos.x > camera.x) && (entpos.x < camera.x + win_size.x) &&
       (entpos.y > camera.y) && (entpos.y < camera.y + win_size.y)))
    return true;
  return false;
}

void BuildingSprite::draw_selection_circle(Camera& camera) const
{
  Position pos(this->building->x * CELL_SIZE + CELL_SIZE/2, this->building->y * CELL_SIZE + CELL_SIZE/2);
  sf::Vector2u entpos = camera.world_to_camera_position(pos);
  const unsigned short width = 65;

  // sf::Vector2f center(entpos.x - width/2 - camera.x, entpos.y - width/2 - camera.y);
  sf::Vector2f center(entpos.x - camera.x, entpos.y - camera.y);

  sf::CircleShape circle;
  circle.setRadius(width);
  circle.setOrigin(width, width);
  circle.setOutlineColor(sf::Color(0xff, 0xcb, 0xF6, 255));
  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineThickness(2);
  circle.setPosition(center);
  circle.setScale(sf::Vector2f(1, 3.f/4.f));
  camera.draw(circle);
}
