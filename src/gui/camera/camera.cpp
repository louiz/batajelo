#include <gui/camera/camera.hpp>
#include <gui/camera/layer.hpp>

Camera::Camera(ClientWorld* world, GraphMap* map, sf::RenderWindow* win):
  x(0),
  y(0),
  zoom(1),
  focused(true),
  movement_speed(55),
  previous_position(0, 0),
  start_drag_position(0, 0),
  world(world),
  map(map),
  win(win)
{
}

Camera::~Camera()
{
}

void Camera::draw()
{
  this->draw_map();
}

void Camera::draw_entity(const Entity* entity)
{
  // TODO check against its position, etc.
  sf::CircleShape circle;
  circle.setRadius(entity->width / 2);
  if (entity->is_selected())
    circle.setOutlineColor(sf::Color::Red);
  else
    circle.setOutlineColor(sf::Color::Blue);
  circle.setOutlineThickness(5);
  circle.setPosition(entity->x, entity->y);
  this->win->draw(circle);
  std::ostringstream os;
  os << entity->health;
  sf::Text health_text(os.str());
  health_text.setPosition(entity->x - 5, entity->y - 40);
  this->win->draw(health_text);
}

void Camera::handle_event(const sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == 0)
    {
      this->previous_position = sf::Mouse::getPosition(*this->win);
      this->start_drag_position = this->previous_position;
      this->win->setMouseCursorVisible(false);
    }
  if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle)
    {
      sf::Mouse::setPosition(this->start_drag_position);
      this->win->setMouseCursorVisible(true);
      this->previous_position = sf::Vector2i(0, 0);
    }
}

void Camera::update(const Duration& dt)
{
  sf::Vector2i pos = sf::Mouse::getPosition(*this->win);
  const sf::Vector2u win_size = this->win->getSize();
  if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) &&
      (this->previous_position != sf::Vector2i(0, 0)))
    {
      this->x -= 2 * this->movement_speed * sec(dt) * (this->previous_position.x - pos.x);
      this->y -= 2 * this->movement_speed * sec(dt) * (this->previous_position.y - pos.y);
      this->previous_position = sf::Vector2i(pos.x, pos.y);
    }
  else if (this->focused)
    {
      if ((pos.x > 0) && (static_cast<uint>(pos.x) > (win_size.x - 2)))
        pos.x = (win_size.x - 2);
      if (pos.x < 1)
        pos.x = 1;
      if ((pos.y > 0) && (static_cast<uint>(pos.y) > (win_size.y - 2)))
        pos.y = (win_size.y - 2);
      if (pos.y < 1)
        pos.y = 1;
      sf::Mouse::setPosition(pos, *this->win);
      if (pos.x < 100)
        this->x -= this->movement_speed * sec(dt);
      if (pos.y < 100)
        this->y -= this->movement_speed * sec(dt);
      if ((pos.y > 0) && static_cast<uint>(pos.y) > (win_size.y - 100))
        this->y += this->movement_speed * sec(dt);
      if ((pos.x > 0) && static_cast<uint>(pos.x) > (win_size.x - 100))
        this->x += this->movement_speed * sec(dt);
    }
  if (this->x < 0)
    this->x = 0;
  else if (this->x > (this->map->get_width() - win_size.x))
    this->x = this->map->get_width() - win_size.x;
  if (this->y < 0)
    this->y = 0;
  else if (this->y > (this->map->get_height() - win_size.y))
    this->y = this->map->get_height() - win_size.y;
}

void Camera::draw_map()
{
  const sf::Vector2u win_size = this->win->getSize();
  Layer* layer;
  uint yoffset;
  uint xoffset = 0;
  /**
   * The position of the layer tile containing the top left
   * corner of the camera.
   */
  uint start_y = static_cast<uint>(this->y) / TILE_HEIGHT * 2;
  if (start_y > 0)
    start_y--;
  start_y -= start_y % 2;
  uint end_y = start_y + win_size.y / TILE_HEIGHT * 2 + 4;
  uint start_x = static_cast<uint>(this->x) / TILE_WIDTH;
  if (start_x > 0)
    start_x--;
  uint end_x = start_x + win_size.x / TILE_WIDTH + 4;

  GraphTile* tile;
  uint level = 0;
  this->world->sort_entities();
  this->world->reset_entity_iterator();

  this->map->reset_layers_iterator();
  while ((layer = this->map->get_next_layer()) != 0)
    {
      if (layer->cells == 0)
        continue ;
      if (end_y >= layer->height)
        end_y = layer->height - 1;
      if (end_x >= layer->width * 2)
        end_x = layer->width * 2 - 1;
      xoffset = 0;
      yoffset = level++ * LEVEL_HEIGHT;
      for (uint y = start_y;
           y < end_y;
           y++)
        {
          for (uint x = start_x;
               x < end_x;
               x++)
            {
              const uint gid = layer->cells[layer->width * y + x];
              tile = this->map->tiles[gid];
              if (tile != 0)
                {
                  tile->sprite.setPosition(x * TILE_WIDTH + xoffset - this->x,
                                           y * TILE_HEIGHT/2 - yoffset - this->y);
                  this->win->draw(tile->sprite);
                }
            }
          if (xoffset != 0)
            xoffset = 0;
          else
            xoffset = TILE_WIDTH/2;
        }
    }
}
