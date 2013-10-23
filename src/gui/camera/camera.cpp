#include <logging/logging.hpp>
#include <gui/camera/camera.hpp>
#include <gui/screen/screen.hpp>
#include <world/layer.hpp>

Camera::Camera(ClientWorld* world, GraphMap* map, sf::RenderWindow* win, Screen* screen):
  x(0),
  y(0),
  zoom(1),
  focused(true),
  movement_speed(0.34),
  previous_position(0, 0),
  start_drag_position(0, 0),
  world(world),
  map(map),
  win(win),
  screen(screen)
{
}

Camera::~Camera()
{
}

void Camera::draw_unit(const Unit* unit, const uint x, const uint y,
                       const bool in_mouse_selection, sf::RectangleShape& rectangle)
{
  if (in_mouse_selection)
    {
      sf::CircleShape in_mouse_circle;
      in_mouse_circle.setRadius(unit->width/2 + 6);
      in_mouse_circle.setOutlineColor(sf::Color(0xff, 0xcb, 0x36, 255));
      in_mouse_circle.setFillColor(sf::Color::Transparent);
      in_mouse_circle.setOutlineThickness(2);
      in_mouse_circle.setPosition(x - unit->width/2 - 6, y - unit->width + 4);
      in_mouse_circle.setScale(sf::Vector2f(1, 3.f/4.f));
      this->win->draw(in_mouse_circle);
    }
  if (this->world->is_entity_selected(unit))
    {
      sf::CircleShape selection_circle;
      selection_circle.setRadius(unit->width/2 + 8);
      selection_circle.setOutlineColor(sf::Color(0x00, 0x00, 0xff, 200));
      selection_circle.setFillColor(sf::Color::Transparent);
      selection_circle.setOutlineThickness(3);
      selection_circle.setPosition(x - unit->width/2 - 8, y - unit->width + 5);
      selection_circle.setScale(sf::Vector2f(1, 3.f/4.f));
      this->win->draw(selection_circle);
    }
  rectangle.setPosition(x - unit->width/2, y - unit->width);
  this->win->draw(rectangle);
}

bool Camera::handle_event(const sf::Event& event)
{
  if (event.type == sf::Event::LostFocus)
    this->focused = false;
  else if (event.type == sf::Event::GainedFocus)
    this->focused = true;
  else if (event.type == sf::Event::MouseButtonPressed)
    {
      switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
          this->handle_left_click(event);
          break;
        case sf::Mouse::Middle:
          this->handle_middle_click(event);
          break;
        case sf::Mouse::Right:
          this->handle_right_click(event);
          break;
        default:
          log_debug("Mouse button not implemented.");
        }
    }
  else if (event.type == sf::Event::MouseButtonReleased)
    {
      switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
          this->handle_left_release(event);
          break;
        case sf::Mouse::Middle:
          this->handle_middle_release(event);
          break;
        default:
          log_debug("Mouse button not implemented.");
        }
    }
  // if (event.type == sf::Event::MouseButtonPressed &&
  //     event.mouseButton.button == sf::Mouse::Middle)
  //   {
  //     this->previous_position = sf::Mouse::getPosition(*this->win);
  //     this->start_drag_position = this->previous_position;
  //   }
  // else if (event.type == sf::Event::MouseButtonReleased &&
  //          event.mouseButton.button == sf::Mouse::Middle)
  //   {
  //     sf::Mouse::setPosition(this->start_drag_position);
  //     this->previous_position = sf::Vector2i(0, 0);
  //   }
  this->fixup_camera_position();
  if (this->mouse_selection.ongoing == true)
    return true;
  return false;
}

void Camera::handle_middle_click(const sf::Event& event)
{
}

void Camera::handle_right_click(const sf::Event& event)
{
  const Position pos = this->camera_to_world_position(event.mouseButton.x,
                                                      event.mouseButton.y);
  // A right click when there's an action associated with the left click
  // just resets the default action of the right click.
  if (this->screen->get_left_click().callback)
    this->screen->reset_left_click_action();
  else // otherwise it always does the move action.
    this->world->action_move(pos.x.toLong(), pos.y.toLong());
}

void Camera::handle_left_click(const sf::Event& event)
{
  // TODO do nothing if the click is not in the camera zone.
  if (!this->screen->get_left_click().callback)
    {
      const sf::Vector2i pos(event.mouseButton.x + this->x,
                             event.mouseButton.y + this->y);

      this->mouse_selection.start(pos);
    }
  else
    {
      const Position pos = this->camera_to_world_position(event.mouseButton.x,
                                                          event.mouseButton.y);
      if (this->screen->get_left_click().callback(pos.x.toLong(), pos.y.toLong(), this->screen->get_left_click().id) == true)
        this->screen->reset_left_click_action();
    }
}

void Camera::handle_left_release(const sf::Event& event)
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    this->add_mouse_selection_to_selection();
  else
    this->set_mouse_selection_to_selection();
}

void Camera::set_mouse_selection_to_selection()
{

  Unit* entity;
  sf::Vector2i mouse_pos = sf::Mouse::getPosition(*this->win);
  mouse_pos.x += this->x;
  mouse_pos.y += this->y;
  // First check the number of entities inside the selection. If it's 0, do
  // nothing
  unsigned int number_of_units = 0;
  for (std::list<Unit*>::iterator it = this->world->units.begin(); it != this->world->units.end(); ++it)
    if (this->mouse_selection.contains(mouse_pos,
                                       this->world_to_camera_position((*it)->pos),
                                       (*it)->width + 4))
      number_of_units++;
  unsigned int number_of_buildings = 0;
  for (std::list<Building*>::iterator it = this->world->buildings.begin(); it != this->world->buildings.end(); ++it)
    {
      Position pos((*it)->x * CELL_SIZE + CELL_SIZE / 2,
                   (*it)->y * CELL_SIZE + CELL_SIZE / 2);
      if (this->mouse_selection.contains(mouse_pos, this->world_to_camera_position(pos)))
        number_of_buildings++;
    }
  log_warning("in mouse selection: Number of units: " << number_of_units << " and buildings: " << number_of_buildings);
  if (number_of_buildings + number_of_units > 0)
    {
      bool new_unit_was_selected = false;
      for (std::list<Unit*>::iterator it = this->world->units.begin(); it != this->world->units.end(); ++it)
        {
          if (this->mouse_selection.contains(mouse_pos,
                                             this->world_to_camera_position((*it)->pos),
                                             (*it)->width + 4))
            {
              if (this->world->is_entity_selected(*it) == false)
                {
                  new_unit_was_selected = true;
                  this->world->select_entity(*it);
                }
            }
          else
            {
              if (this->world->is_entity_selected(*it) == true)
                this->world->unselect_entity(*it);
            }
        }
      for (std::list<Building*>::iterator it = this->world->buildings.begin(); it != this->world->buildings.end(); ++it)
        {
          Position pos((*it)->x * CELL_SIZE + CELL_SIZE / 2,
                       (*it)->y * CELL_SIZE + CELL_SIZE / 2);
          if (this->mouse_selection.contains(mouse_pos, this->world_to_camera_position(pos)))
            {
              if (this->world->is_entity_selected(*it) == false)
                {
                  // We select the building only if no unit was added to the
                  // selection.  This way we can box an area and select only the units
                  // inside it, because that's what we want most of the time when
                  // doing that.
                  if (new_unit_was_selected == false)
                    this->world->select_entity(*it);
                }
            }
          else
            {
              if (this->world->is_entity_selected(*it) == true)
                this->world->unselect_entity(*it);
            }
        }
    }
  this->mouse_selection.end();
}

void Camera::add_mouse_selection_to_selection()
{
  Unit* entity;
  sf::Vector2i mouse_pos = sf::Mouse::getPosition(*this->win);
  mouse_pos.x += this->x;
  mouse_pos.y += this->y;
  for (std::list<Unit*>::iterator it = this->world->units.begin(); it != this->world->units.end(); ++it)
    {
      if (this->mouse_selection.contains(mouse_pos,
                                         this->world_to_camera_position((*it)->pos),
                                         (*it)->width + 4))
        {
          if (this->world->is_entity_selected(*it) == false)
            this->world->select_entity(*it);
        }
    }
  this->mouse_selection.end();
}

void Camera::handle_middle_release(const sf::Event& event)
{
}

void Camera::update(const Duration& dt)
{
  sf::Vector2i pos = sf::Mouse::getPosition(*this->win);
  const sf::Vector2u win_size = this->win->getSize();
  if (this->focused)
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
      if (pos.x < 20)
        this->x -= this->movement_speed * sec(dt);
      if (pos.y < 20)
        this->y -= this->movement_speed * sec(dt);
      if ((pos.y > 0) && static_cast<uint>(pos.y) > (win_size.y - 20))
        this->y += this->movement_speed * sec(dt);
      if ((pos.x > 0) && static_cast<uint>(pos.x) > (win_size.x - 20))
        this->x += this->movement_speed * sec(dt);
    }
  this->fixup_camera_position();
}

void Camera::draw()
{
  const sf::Vector2u win_size = this->win->getSize();
  Layer* layer;
  uint yoffset;
  uint xoffset = 0;
  // The position of the layer tile containing the top left corner of the
  //camera.
  uint start_y = static_cast<uint>(this->y) / TILE_HEIGHT;
  uint end_y = start_y + win_size.y / TILE_HEIGHT + 2;
  uint start_x = static_cast<uint>(this->x) / TILE_WIDTH;
  uint end_x = start_x + win_size.x / TILE_WIDTH + 2;
  short cellx;
  short celly;
  this->world->sort_entities();

  GraphTile* tile;
  std::vector<std::vector<Entity*> > entities;
  entities.resize(5);
  Unit* entity;

  sf::Vector2i mouse_pos = sf::Mouse::getPosition(*this->win);
  mouse_pos.x += this->x;
  mouse_pos.y += this->y;

  sf::RectangleShape rectangle(sf::Vector2f(30, 30));
  rectangle.setOutlineColor(sf::Color::Blue);
  rectangle.setOutlineThickness(1);

  int level;
  for (uint y = start_y;
       y < end_y;
       y++)
    {
      this->map->reset_layers_iterator();
      level = 0;
      yoffset = (level * LEVEL_HEIGHT);

      while ((layer = this->map->get_next_layer()) != 0)
        {
          if (layer->cells == 0)
            {
              level++;
              continue;
            }
          yoffset = (level * LEVEL_HEIGHT);
          for (uint x = start_x;
               x < end_x;
               x++)
            {
              const uint gid = layer->cells[layer->width * y + x];
              tile = this->map->tiles[gid];
              if (tile != 0)
                {
                  tile->sprite.setColor(sf::Color::White);
                  for (std::list<std::size_t>::iterator it = this->world->current_path.begin();
                       it != this->world->current_path.end(); ++it)
                    {
                      if ((*it) == layer->width * y + x)
                        tile->sprite.setColor(sf::Color::Blue);
                    }
                  tile->sprite.setPosition(x * TILE_WIDTH - this->x,
                                           -64 + y * TILE_HEIGHT - yoffset - this->y);
                  this->win->draw(tile->sprite);
                }
            }
          Building* building;
          for (std::list<Building*>::iterator it = this->world->buildings.begin(); it != this->world->buildings.end(); ++it)
            {
              building = *it;
              this->screen->building_sprites[building->type_id - this->world->number_of_units_models()]->draw(this, this->world, this->screen, building);

              // this->world->get_cell_at_position(pos, cellx, celly);
              // sf::Vector2u entpos = this->world_to_camera_position(pos);
              // if ((celly == y) && ((entpos.x > this->x) && (entpos.x < this->x + win_size.x) &&
              //                      (entpos.y > this->y) && (entpos.y < this->y + win_size.y)))
              //   {
                  // actually call sprite->draw. Let the BuildingSprite draw itself at the position.
                  // sf::Sprite sprite = this->screen->building_sprites[building->type_id - this->world->number_of_units_models()]->get_cursor_sprite();
                  // this->win->draw(sprite);
                  // this->draw_unit(entity, entpos.x - this->x, entpos.y - this->y,
                  //                   this->mouse_selection.contains(mouse_pos,
                  //                                                  entpos, entity->width + 4) ||
                  //                   this->screen->is_entity_hovered(entity),
                  //                   rectangle);
                // }
            }
          // Draw entites on that line.
          std::vector<Entity*> entities_at_that_level = entities[level];
          int i = 0;
          for (std::list<Unit*>::iterator it = this->world->units.begin(); it != this->world->units.end(); ++it)
            {
              entity = *it;
              this->world->get_cell_at_position(entity->pos, cellx, celly);
              sf::Vector2u entpos = this->world_to_camera_position(entity->pos);
              if ((celly == y) && ((entpos.x > this->x) && (entpos.x < this->x + win_size.x) &&
                                   (entpos.y > this->y) && (entpos.y < this->y + win_size.y)))
                {
                  this->draw_unit(entity, entpos.x - this->x, entpos.y - this->y,
                                  this->mouse_selection.contains(mouse_pos,
                                                                 entpos, entity->width + 4) ||
                                  this->screen->is_entity_hovered(entity),
                                  rectangle);
                }
            }
          level++;
        }
    }
  this->draw_mouse_selection();
}

void Camera::draw_mouse_selection()
{
  if (this->mouse_selection.ongoing == false)
    return ;
  sf::Vector2i mouse_pos = sf::Mouse::getPosition(*this->win);
  mouse_pos.x += this->x;
  mouse_pos.y += this->y;
  sf::RectangleShape rect(sf::Vector2f(::abs(mouse_pos.x - this->mouse_selection.start_pos.x),
                                       ::abs(mouse_pos.y- this->mouse_selection.start_pos.y)));
  rect.setOutlineThickness(1);
  rect.setFillColor(sf::Color(13, 0, 152, 40));
  rect.setOutlineColor(sf::Color(13, 0, 152, 0x36));
  sf::Vector2f rect_pos;
  if (this->mouse_selection.start_pos.x < mouse_pos.x)
    rect_pos.x = this->mouse_selection.start_pos.x - this->x;
  else
    rect_pos.x = mouse_pos.x - this->x;
  if (this->mouse_selection.start_pos.y < mouse_pos.y)
    rect_pos.y = this->mouse_selection.start_pos.y - this->y;
  else
    rect_pos.y = mouse_pos.y - this->y;
  rect.setPosition(rect_pos);
  this->win->draw(rect);
}

void Camera::fixup_camera_position()
{
  const sf::Vector2u win_size = this->win->getSize();
  if (this->x < 0)
    this->x = 0;
  else if (this->x > (this->map->get_width_in_pixels() - win_size.x))
    this->x = this->map->get_width_in_pixels() - win_size.x;
  if (this->y < 0)
    this->y = 0;
  else if (this->y > (this->map->get_height_in_pixels() - win_size.y))
    this->y = this->map->get_height_in_pixels() - win_size.y;
}

sf::Vector2u Camera::world_to_camera_position(const Position& pos) const
{
  sf::Vector2u res;
  res.x = (pos.x.toLong() * TILE_WIDTH) / CELL_SIZE;
  res.y = (pos.y.toLong() * TILE_HEIGHT) / CELL_SIZE;
  mpreal height = this->world->get_position_height(pos) * 32;
  res.y -= height.toLong();
  return res;
}

Position Camera::camera_to_world_position(const int x,
                                          const int y) const
{
  Position res;
  const uint cell_size = static_cast<const uint>(CELL_SIZE);
  res.x = (x + this->x) * (CELL_SIZE / static_cast<const float>(TILE_WIDTH));
  res.y = (y + this->y) * (CELL_SIZE / static_cast<const float>(TILE_HEIGHT));

  uint offset = (cell_size - (res.y.toLong() % cell_size));
  uint i = 0;
  assert(offset <= 32767);
  mpreal height_of_bottom_cell = this->world->get_position_height(Position(res.x, res.y + static_cast<short>(offset)));
  if (height_of_bottom_cell > ((offset) * (1.f/LAYER_HEIGHT)))
    res.y += (height_of_bottom_cell * LAYER_HEIGHT).toLong();
  else
    {
      offset += cell_size;
      height_of_bottom_cell = this->world->get_position_height(Position(res.x, res.y + static_cast<short>(offset)));
      if (height_of_bottom_cell > (offset * (1.f/LAYER_HEIGHT)))
        res.y += (height_of_bottom_cell * LAYER_HEIGHT).toLong();
      else
        {
          mpreal height_of_current_cell = this->world->get_position_height(Position(res.x, res.y));
          res.y += (height_of_current_cell * LAYER_HEIGHT).toLong();
        }
    }
  return res;
}

bool Camera::is_mouse_selection_ongoing() const
{
  return this->mouse_selection.ongoing;
}

void Camera::draw(const sf::Drawable& drawable)
{
  this->win->draw(drawable);
}

const sf::Vector2u Camera::get_win_size() const
{
  return this->win->getSize();
}
