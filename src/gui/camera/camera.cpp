#include <logging/logging.hpp>
#include <gui/camera/camera.hpp>
#include <gui/screen/screen.hpp>
#include <world/layer.hpp>
#include <gui/sprites/archive_sprite.hpp>
#include <gui/sprites/pic_sprite.hpp>

Camera::Camera(ClientWorld* world, GraphMap* map, sf::RenderWindow* win, Screen* screen):
  x(0),
  y(0),
  zoom(1),
  focused(true),
  movement_speed(0.023),
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
  GraphTile* tile;

  uint layer_width = this->map->twidth;
  uint layer_height = this->map->theight;
  uint row_number = layer_height + layer_width - 1;

  auto unit_it = this->world->units.begin();

  uint row;
  bool upper_part;
  for (row = 0; row < row_number; ++row)
    {
      upper_part = row < layer_height;
      uint yoffset = -LEVEL_HEIGHT;
      for (Layer* layer: this->map->layers)
        {
          yoffset += LEVEL_HEIGHT;
          if (!layer->cells)
            continue;
          uint col = 0;
          int start_col;
          int end_col;
          const int step = layer_width - 1;
          if (upper_part)
            {
              start_col = row * layer_width;
              end_col = 0;
            }
          else
            {
              start_col = (layer_height - 1) * layer_width + row - layer_width + 1;
              end_col = layer_width - 1 + (row - layer_height + 1) * layer_width - 1;
            }
          if (start_col == 0)
            end_col = -1;
          for (int n = start_col; n > end_col; n -= step)
            {
              ++col;
              std::size_t gid = layer->cells[n];
              tile = this->map->tiles[gid];
              if (tile)
                {
                  uint sx = ((96 * layer_width) / 2) - (row * 96/2) + col * 96;
                  if (!upper_part)
                    sx += 96 * (row - layer_height + 1);
                  const uint sy = (72 / 2) * row;
                  tile->sprite.setPosition(sx - this->x, sy - this->y - yoffset);
                  this->win->draw(tile->sprite);
                }
            }
        }
      for (Sprite* sprite: this->sprites)
        {
          sprite->draw(this, this->world, this->screen);
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

/**
 * Values used to make an angular rotation
 * See http://en.wikipedia.org/wiki/Rotation_%28mathematics%29#Matrix_algebra
 * or your school lessons.
 */

// sin(7π/4)
static const float sin_45 = 0.6496369390800625;
// cos(7π/4)
static const float cos_45 = 0.7602445970756301;

sf::Vector2u Camera::world_to_camera_position(const Position& pos) const
{
  sf::Vector2u res;
  // Do some proportionality, to convert from the size of the world to the
  // size of the screen
  // res.x = (pos.x.toLong() * TILE_WIDTH) / CELL_SIZE;
  // res.y = (pos.y.toLong() * TILE_HEIGHT) / CELL_SIZE;

  res.x = pos.x.toLong();
  res.y = pos.y.toLong();
  // Then a rotation of a 45° angle
  log_error("Before rotate: " << res.x << ": " << res.y);
  res.x = res.x * cos_45 - res.y * sin_45;
  res.y = res.y * sin_45 + res.y * cos_45;
  log_error("After rotate: " << res.x << ": " << res.y);

  // A translation to the right, because the world and the screen do not have
  // the same origin
  

  // Then adjust y using the height of that position in the world
  Fix16 height = this->world->get_position_height(pos) * 32;
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
  Fix16 height_of_bottom_cell = this->world->get_position_height(Position(res.x, res.y + static_cast<short>(offset)));
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
          Fix16 height_of_current_cell = this->world->get_position_height(Position(res.x, res.y));
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

void Camera::on_new_unit(const Unit* unit)
{
  this->sprites.push_back(new PicpicSprite(unit));
}

void Camera::on_new_building(const Building* building)
{
  this->sprites.push_back(new ArchiveSprite(building));
}

const sf::Vector2u Camera::get_win_size() const
{
  return this->win->getSize();
}

void Camera::graphical_tick()
{
  for (Sprite*& sprite: this->sprites)
    {
      sprite->tick();
    }
}

const sf::Vector2i Camera::get_mouse_position() const
{
  return sf::Mouse::getPosition(*this->win);
}
