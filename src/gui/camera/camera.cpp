#include <gui/camera/camera.hpp>
#include <gui/camera/layer.hpp>

#define TILEHEIGHT 96
#define TILEWIDTH 128


Camera::Camera(ClientWorld* world, GraphMap* map):
  x(0),
  y(0),
  zoom(1),
  world(world),
  map(map)
{
}

Camera::~Camera()
{
}

void Camera::draw(sf::RenderWindow* win)
{
  this->draw_map(win);
}

void Camera::draw_entity(sf::RenderWindow* win, const Entity* entity)
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
  win->draw(circle);
  std::ostringstream os;
  os << entity->health;
  sf::Text health_text(os.str());
  health_text.setPosition(entity->x - 5, entity->y - 40);
  win->draw(health_text);
}

void Camera::handle_event(const sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == 0)
    {
      this->world->handle_event(actions::Select, event.mouseButton.x, event.mouseButton.y);
    }
  else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == 1)
    {
      this->world->handle_event(actions::Move, event.mouseButton.x, event.mouseButton.y);
    }
  else if (event.type == sf::Event::KeyPressed && event.key.code == 57)
    {
      this->world->confirm_initial_turn();
      this->world->start();
    }
}

void Camera::update(const Duration& dt)
{
}

void Camera::draw_map(sf::RenderWindow* win)
{
  Layer* layer;
  uint yoffset;
  uint xoffset = 0;
  uint y;
  uint x;
  uint cell;
  GraphTile* tile;
  uint level = 0;

  this->map->reset_layers_iterator();
  while ((layer = this->map->get_next_layer()) != 0)
    {
      if (layer->cells == 0)
        continue ;
      log_debug("layer");
      xoffset = 0;
      cell = 0;
      yoffset = level++ * LEVEL_HEIGHT;
      for (y = 0; y < (layer->height * TILE_HEIGHT/2); y += TILE_HEIGHT/2)
        {
          for (x = 0; x < (layer->width * TILE_WIDTH); x += TILE_WIDTH)
            {
              uint gid = layer->cells[cell++];
              tile = this->map->tiles[gid];
              if (tile != 0)
                {
                  tile->sprite.setPosition(x + xoffset, 50 + y - yoffset);
                  win->draw(tile->sprite);
                }
            }
          if (xoffset != 0)
            xoffset = 0;
          else
            xoffset = TILE_WIDTH/2;
        }
    }
}
