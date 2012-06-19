#include <gui/hud/minimap.hpp>

Minimap::Minimap(sf::RenderWindow* win, GraphMap* map, Camera* camera):
  win(win),
  map(map),
  camera(camera),
  minimap_sprite(map->minimap_texture)
{
  const sf::Vector2u win_size = win->getSize();
  const uint x = MINIMAP_X;
  const uint y = win_size.y - MINIMAP_Y - MINIMAP_HEIGHT;

  log_debug("Minimap. Map size: " <<
            map->get_width_in_tiles() << ":" << map->get_height_in_tiles() <<
            " = " << static_cast<float>(map->get_width_in_tiles()) /
                     static_cast<float>(map->get_height_in_tiles()));
  this->minimap_sprite.setPosition(x, y);
}

Minimap::~Minimap()
{
}

void Minimap::draw()
{
  const sf::Vector2u win_size = this->win->getSize();
  const uint x = MINIMAP_X;
  const uint y = win_size.y - MINIMAP_Y - MINIMAP_HEIGHT;
  const float xratio = MINIMAP_HEIGHT / map->get_height_in_pixels();
  const float yratio = MINIMAP_WIDTH / map->get_width_in_pixels();

  // Draw the borders
  sf::RectangleShape borders(sf::Vector2f(MINIMAP_WIDTH, MINIMAP_HEIGHT));
  borders.setFillColor(sf::Color(40, 40, 40, 230));
  borders.setOutlineThickness(2);
  borders.setOutlineColor(sf::Color::Black);
  borders.setPosition(0, 0);
  borders.move(x, y);
  this->win->draw(borders);

  // Draw the map
  this->win->draw(this->minimap_sprite);

  // Draw the camera
  sf::RectangleShape camera_shape(sf::Vector2f(win_size.x * xratio,
                                         win_size.y * yratio));
  camera_shape.setOutlineThickness(1);
  camera_shape.setFillColor(sf::Color::Transparent);
  camera_shape.setOutlineColor(sf::Color::White);
  camera_shape.setPosition(x + (this->camera->x * xratio),
                           y + (this->camera->y * yratio));
  this->win->draw(camera_shape);
}

bool Minimap::handle_event(const sf::Event& event)
{
  const sf::Vector2u win_size = this->win->getSize();
  const uint x = MINIMAP_X;
  const uint y = win_size.y - MINIMAP_Y - MINIMAP_HEIGHT;
  sf::Vector2i mouse_pos = sf::Mouse::getPosition();

  if ((((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) ||
       ((event.type == sf::Event::MouseMoved) && (sf::Mouse::isButtonPressed(sf::Mouse::Left))))
      &&
      (((mouse_pos.x > x) && (mouse_pos.x < x + MINIMAP_WIDTH)) && ((mouse_pos.y > y ) && (mouse_pos.y < y + MINIMAP_HEIGHT)))
      )
    {
      const float xratio = MINIMAP_HEIGHT / map->get_height_in_pixels();
      const float yratio = MINIMAP_WIDTH / map->get_width_in_pixels();
      this->camera->x = (mouse_pos.x - x) / xratio - (win_size.x / 2);
      this->camera->y = (mouse_pos.y - y) / yratio - (win_size.y / 2);
      return true;
    }
  return false;
}
