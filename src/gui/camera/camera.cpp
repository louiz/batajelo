#include <logging/logging.hpp>
#include <gui/camera/camera.hpp>
#include <gui/screen/screen.hpp>
#include <gui/sprites/pic_sprite.hpp>
#include <world/world.hpp>
#include <world/layer.hpp>
#include <world/entity.hpp>
#include <gui/utils.hpp>
#include <game/game_client.hpp>
#include <climits>
#include <cstdlib>

#include <world/abilities.hpp>
#include <world/location.hpp>

#include <iostream>

Camera::Camera(GameClient* game, Screen* screen):
  ScreenElement(screen),
  x(0),
  y(56),
  zoom(1),
  focused(true),
  movement_speed(1000),
  previous_position(0, 0),
  start_drag_position(0, 0),
  game(game),
  mouse_selection(),
  tileset(),
  fog(1920, 1080, this, &this->world())
{
  this->tileset.load_from_file("test6.tmx");

}

Camera::~Camera()
{
}

World& Camera::world() const
{
  return this->game->get_world();
}

Map& Camera::map() const
{
  return this->world().get_map();
}

sf::RenderWindow& Camera::win()
{
  return this->screen->window();
}

sf::RenderWindow& Camera::win() const
{
  return this->screen->window();
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

void Camera::handle_middle_click(const sf::Event&)
{
}

void Camera::handle_right_click(const sf::Event& event)
{
  const Position pos = this->camera_to_world_position(event.mouseButton.x,
                                                      event.mouseButton.y);
  bool queue = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
  log_debug("queue: " << queue);

  // A right click when there's an action associated with the left click
  // just resets the default action of the right click.
  if (this->screen->get_left_click().callback)
    this->screen->reset_left_click();
  else // otherwise it always does the move action.
    {
      std::vector<EntityId> ids;
      for (const auto& entity: this->game->get_selection().get_entities())
        ids.push_back(entity->get_id());
      if (!ids.empty())
        this->game->action_move(ids, pos, queue);
    }
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
      if (this->screen->get_left_click().callback(pos) == true)
        this->screen->reset_left_click();
    }
}

void Camera::handle_left_release(const sf::Event&)
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    this->add_mouse_selection_to_selection();
  else
    this->set_mouse_selection_to_selection();
}

void Camera::set_mouse_selection_to_selection()
{
  sf::Vector2i mouse_pos = this->get_mouse_position();

  // First check the number of entities inside the selection. If it's 0, do
  // nothing
  auto number_of_entities = std::count_if(this->world().entities.begin(),
                                          this->world().entities.end(),
                                          [this, &mouse_pos](const std::shared_ptr<Entity>& entity) -> bool
                                          {
                                            Location* location = entity->get<Location>();
                                            if (location)
                                              return this->mouse_selection.contains(mouse_pos,
                                                                                    this->world_to_camera_position(location->position()),
                                                                                    location->get_width().to_int());
                                            return false;
                                          });
  auto number_of_buildings = 0;
  log_warning("in mouse selection: Number of entities: " << number_of_entities << " and buildings: " << number_of_buildings);
  if (number_of_buildings + number_of_entities > 0)
    {
      bool new_unit_was_selected = false;
      for (const auto& entity: this->world().entities)
        {
          Location* location = entity->get<Location>();
          if (!location)
            continue;
          if (this->mouse_selection.contains(mouse_pos,
                                             this->world_to_camera_position(location->position()),
                                             location->get_width().to_int() + 4))
            {
              if (this->game->is_entity_selected(entity.get()) == false)
                {
                  new_unit_was_selected = true;
                  this->game->select_entity(entity.get());
                }
            }
          else
            {
              if (this->game->is_entity_selected(entity.get()) == true)
                this->game->unselect_entity(entity.get());
            }
        }
    }
  this->mouse_selection.end();
}

void Camera::add_mouse_selection_to_selection()
{
  sf::Vector2i mouse_pos = this->get_mouse_position();

  for (const auto& entity: this->world().entities)
    {
      Location* location = entity->get<Location>();
      if (!location)
        continue;
      if (this->mouse_selection.contains(mouse_pos,
                                         this->world_to_camera_position(location->position()),
                                         location->get_width().to_int() + 4))
        {
          if (this->game->is_entity_selected(entity.get()) == false)
            this->game->select_entity(entity.get());
        }
    }
  this->mouse_selection.end();
}

void Camera::handle_middle_release(const sf::Event&)
{
}

void Camera::update(const utils::Duration& dt)
{
  sf::Vector2i pos = this->screen->get_mouse_position();

  const sf::Vector2u win_size = this->get_win_size();
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
      // TODO
      // sf::Mouse::setPosition(pos, *this->win);
      if (pos.x < 20)
        this->x -= this->movement_speed * utils::sec(dt).count();
      if (pos.y < 20)
        this->y -= this->movement_speed * utils::sec(dt).count();
      if ((pos.y > 0) && static_cast<uint>(pos.y) > (win_size.y - 20))
        this->y += this->movement_speed * utils::sec(dt).count();
      if ((pos.x > 0) && static_cast<uint>(pos.x) > (win_size.x - 20))
        this->x += this->movement_speed * utils::sec(dt).count();
    }
  this->fixup_camera_position();
}

void Camera::draw()
{
  const sf::Vector2i mouse_pos = this->get_mouse_position();

  // Sort the sprites by their vertical position
  this->sprites.sort([](const auto& a, const auto& b)
                     {
                       return a->get_world_pos().y < b->get_world_pos().y;
                     });

  // Draw all tiles even the ones that are not visible for the camera.
  GraphTile* tile;
  // Iterate all the rows, from top to bottom
  for (unsigned short row = 0; row < this->map().get_height_in_tiles(); row++)
    {
      // Position where we will draw, in pixels
      unsigned int y = row * (TILE_HEIGHT / 2);
      unsigned int x;
      // Draw the current row of all layers, from bottom to top
      for (Layer* layer: this->map().layers)
        {
          if (layer && layer->cells)
            for (unsigned short col = 0; col < this->map().get_width_in_tiles(); col++)
              {
                x = col * TILE_WIDTH;
                if (row % 2 != 0)
                  x += HALF_TILE_W;
                std::size_t cell = col + (row * this->map().get_height_in_tiles());
                std::size_t gid = layer->cells[cell];
                tile = this->tileset.tiles[gid].get();
                if (tile)
                  {
                    constexpr char tile_opacity = 180;
                    tile->sprite.setColor(sf::Color(255, 255, 255, tile_opacity));
                    if (std::find(this->world().current_path.begin(), this->world().current_path.end(),
                                  this->map().cell_to_index(std::make_tuple(col, row)))
                        != this->world().current_path.end())
                      tile->sprite.setColor(sf::Color(0, 120, 255, 255));

                    tile->sprite.setPosition(x - this->x, y - this->y);
                    this->win().draw(tile->sprite);
                  }
              }
          // The row for the next layer must be shifted from a few pixel to
          // the top
          y -= LEVEL_HEIGHT;
        }
      // Display all the sprites that are on this row
      for (const auto& sprite: this->sprites)
        {
          Position sprite_world_position = sprite->get_world_pos();
          unsigned short x;
          unsigned short y;
          std::tie(x, y) = this->world().get_cell_at_position(sprite_world_position);
          if (y == row &&
              this->world().can_be_seen_by_team(sprite_world_position, this->game->get_self_team()))
            {
              if (this->mouse_selection.contains(mouse_pos, this->world_to_camera_position(sprite_world_position)))
                this->draw_hover_indicator(this->world_to_camera_position(sprite_world_position), 80);
              if (this->get_game_client()->is_entity_selected(sprite->get_entity()))
                this->draw_selected_indicator(this->world_to_camera_position(sprite_world_position), 80);
              sprite->draw(this->game);
            }
        }
    }

  this->draw_mouse_selection();

  // Debug
  const std::string cam_pos = "Camera position: " + std::to_string(this->x) + ", " + std::to_string(this->y);
  this->game->get_debug_hud().add_debug_line(cam_pos);
  this->game->get_debug_hud().add_debug_line("Map width in pixel: " + std::to_string(this->map().get_width_in_pixels()) + ", " + std::to_string(this->map().get_height_in_pixels()));

  Position world_mouse_pos = this->camera_to_world_position(mouse_pos.x, mouse_pos.y);
  this->game->get_debug_hud().add_debug_line("Mouse world position: " +
                                               std::to_string(world_mouse_pos.x.to_double()) + ", " +
                                               std::to_string(world_mouse_pos.y.to_double()));

  this->draw(this->fog.get_sprite());
}

void Camera::draw_mouse_selection()
{
  if (this->mouse_selection.ongoing == false)
    return ;
  sf::Vector2i mouse_pos = this->get_mouse_position();

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
  this->win().draw(rect);
}

void Camera::draw_hover_indicator(const sf::Vector2i& center, const unsigned int width)
{
  const float radius = static_cast<float>(width) / 2.0;
  sf::CircleShape circle(radius, 6);
  circle.setOutlineColor(sf::Color::Red);
  circle.setFillColor({0, 0, 0, 0});
  circle.setOutlineThickness(2);
  circle.scale(1.0f, 0.666f);
  circle.setPosition(static_cast<float>(center.x) - radius - this->x,
                     static_cast<float>(center.y) - radius * 0.666f - this->y);
  this->draw(circle);
}

void Camera::draw_selected_indicator(const sf::Vector2i& center, const unsigned int width)
{
  const float radius = static_cast<float>(width) / 2.0;
  sf::CircleShape circle(radius);
  circle.setOutlineColor(sf::Color::Blue);
  circle.setFillColor({0, 0, 0, 0});
  circle.setOutlineThickness(2);
  circle.scale(1.0f, 0.666f);
  circle.setPosition(static_cast<float>(center.x) - radius - this->x,
                     static_cast<float>(center.y) - radius * 0.666f - this->y);
  this->draw(circle);
}

void Camera::fixup_camera_position()
{
  const sf::Vector2u win_size = this->win().getSize();
  if (this->x < 0)
    this->x = 0;
  else if (this->x > (this->map().get_width_in_pixels() - win_size.x))
    this->x = this->map().get_width_in_pixels() - win_size.x;
  if (this->y < 0)
    this->y = 0;
  else if (this->y > (this->map().get_height_in_pixels() - win_size.y))
    this->y = this->map().get_height_in_pixels() - win_size.y;
}

sf::Vector2i Camera::world_to_camera_position(const Position& pos) const
{
  sf::Vector2i res;

  res.x = pos.x.to_int();
  res.y = pos.y.to_int();

  res.x = (res.x * TILE_WIDTH) / CELL_SIZE;
  res.y = (res.y * TILE_HEIGHT) / CELL_SIZE;

  // Then adjust y using the height of that position in the world
  Fix16 height = this->world().get_position_height(pos) * 24;

  res.y += TILE_TOP_OFFSET - height.to_int();

  return res;
}

CellIndex Camera::get_cell_at_coord(const unsigned int x, const unsigned int y) const
{
  const Position pos(x * CELL_SIZE / TILE_WIDTH,
                     y * CELL_SIZE / TILE_HEIGHT);

  const Cell cell = this->world().get_cell_at_position(pos);
  return this->map().cell_to_index(cell);
}

float get_edge_height(const float x, const float left_height, const float right_height)
{
  if (left_height == right_height)
    return left_height;

  const float ratio = x / HALF_TILE_W;
  if (left_height > right_height)
    return left_height - (ratio * (left_height - right_height));
  else
    return left_height + (ratio * (right_height - left_height));
}

sf::Vector2i Camera::get_mouse_position() const
{
  auto res = this->screen->get_mouse_position();
  res.x += this->x;
  res.y += this->y;
  return res;
}

Position Camera::camera_to_world_position(const int x, const int y) const
{
  // The empty space at the top of the world, due to tile size. In pixels
  static const unsigned int top_offset = TILE_TOP_OFFSET;

  if (x + this->x <= 0 ||
      y + this->y - top_offset <= 0)
    return Position::zero;

  static const float cell_size = static_cast<const float>(CELL_SIZE);

  // Get the cell (at the floor level) at the position
  const CellIndex cell_index = this->get_cell_at_coord(x + this->x,
                                                       y + this->y - top_offset);

  // DEBUG only
  selected_cell = cell_index;

  if (cell_index == InvalidCellIndex)
    return Position::invalid;

  unsigned int col;
  unsigned int row;
  std::tie(col, row) = this->map().index_to_cell(cell_index);

  // If the world was completely flat, this would be the result position
  Position res;
  res.x = (x + this->x) * (cell_size / TILE_WIDTH);
  res.y = (y + this->y - top_offset) * (cell_size / TILE_HEIGHT);

  // Now we need to check if, due to the levels of the world, this position
  // is hidden or not by a surrounding cell. If yes, the position is
  // actually on this other cell.

  // If we are on the left side of the cell, we may be hidden by the bottom
  // or the left cells. If we are on the right side, the bottom or the right
  // cells.
  unsigned int xx = x + this->x;
  if (row % 2)
    xx += HALF_TILE_W;

  // TODO this in a (World::?) function
  TileHeights heights = this->map().get_cell_heights(col, row);

  this->game->get_debug_hud().add_debug_line("Current cell corner heights: " +
                                               std::to_string(heights.corners.left) + ", " +
                                               std::to_string(heights.corners.top) + ", " +
                                               std::to_string(heights.corners.right) + ", " +
                                               std::to_string(heights.corners.bottom));

  // Relative x and y position on the base square of the grid-aligned tile
  unsigned int rel_x = (x + this->x) - (TILE_WIDTH * col);
  unsigned int rel_y = (y + this->y - top_offset) - ((TILE_HEIGHT / 2) * row);
  if (row % 2)
    rel_x -= HALF_TILE_W;

  float adj_rel_x;
  float edge_height;
  float bot_edge_height;
  // TODO fix the height. It should not be (bot + top)/2
  float height = 0;
  CellIndex adj_cell;

  if (xx % TILE_WIDTH >= HALF_TILE_W)
    {
      // RIGHT
      adj_rel_x = rel_x - HALF_TILE_W;

      edge_height = ::get_edge_height(adj_rel_x,
                                      heights.corners.top, heights.corners.right);
      bot_edge_height = ::get_edge_height(adj_rel_x,
                                          heights.corners.bottom, heights.corners.right);
      height = (edge_height + bot_edge_height) / 2;

      // Get the bottom-right cell (adj_cell) and its heights
      if (row % 2 == 0)
        adj_cell = col + (row + 1) * this->map().get_width_in_tiles();
      else
        adj_cell = col + 1 + (row + 1) * this->map().get_width_in_tiles();
      heights = this->map().get_cell_heights(adj_cell);

      edge_height = ::get_edge_height(adj_rel_x,
                                      heights.corners.left, heights.corners.top);
      bot_edge_height = ::get_edge_height(adj_rel_x,
                                          heights.corners.left, heights.corners.bottom);

      if ((edge_height * LAYER_HEIGHT) + adj_rel_x * TILE_HW_RATIO > (TILE_HEIGHT - rel_y))
        {
          selected_cell = adj_cell;
          height = (edge_height + bot_edge_height) / 2;
        }

      // Now do the “same” thing with the cell at the bottom
      adj_cell = col + (row + 2) * this->map().get_width_in_tiles();
      heights = this->map().get_cell_heights(adj_cell);

      edge_height = ::get_edge_height(adj_rel_x,
                                       heights.corners.top, heights.corners.right);
      bot_edge_height = ::get_edge_height(adj_rel_x,
                                           heights.corners.bottom, heights.corners.right);

      if ((edge_height * LAYER_HEIGHT) - adj_rel_x * TILE_HW_RATIO > (TILE_HEIGHT - rel_y))
        {
          selected_cell = adj_cell;
          height = (edge_height + bot_edge_height) / 2;
        }
    }
  else
    {
      // LEFT
      adj_rel_x = static_cast<float>(rel_x);

      edge_height = ::get_edge_height(adj_rel_x,
                                      heights.corners.left, heights.corners.top);
      bot_edge_height = ::get_edge_height(adj_rel_x,
                                          heights.corners.left, heights.corners.bottom);
      height = (edge_height + bot_edge_height) / 2;

      if (row % 2 == 0)
          adj_cell = col - 1 + (row + 1) * this->map().get_width_in_tiles();
        else
          adj_cell = col + (row + 1) * this->map().get_width_in_tiles();
      heights = this->map().get_cell_heights(adj_cell);

      edge_height = ::get_edge_height(adj_rel_x,
                                      heights.corners.top, heights.corners.right);
      bot_edge_height = ::get_edge_height(adj_rel_x,
                                          heights.corners.bottom, heights.corners.right);

      if (edge_height * LAYER_HEIGHT + (HALF_TILE_W - adj_rel_x) * TILE_HW_RATIO > TILE_HEIGHT - rel_y)
        {
          selected_cell = adj_cell;
          height = (edge_height + bot_edge_height) / 2;
        }

      // Now do the “same” thing with the cell at the bottom
      adj_cell = col + (row + 2) * this->map().get_width_in_tiles();
      heights = this->map().get_cell_heights(adj_cell);

      edge_height = ::get_edge_height(adj_rel_x,
                                     heights.corners.left, heights.corners.top);
      bot_edge_height = ::get_edge_height(adj_rel_x,
                                     heights.corners.left, heights.corners.bottom);

      if ((edge_height * LAYER_HEIGHT) - (HALF_TILE_W - adj_rel_x) * TILE_HW_RATIO > (TILE_HEIGHT - rel_y))
        {
          selected_cell = adj_cell;
          height = (edge_height + bot_edge_height) / 2;
        }
    }

  res.y += height / TILE_HW_RATIO * static_cast<float>(LAYER_HEIGHT);
  this->game->get_debug_hud().add_debug_line("World position: " + std::to_string(res.x.to_double()) + ":" + std::to_string(res.y.to_double()));

  return res;
}

bool Camera::is_mouse_selection_ongoing() const
{
  return this->mouse_selection.ongoing;
}

void Camera::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
  this->win().draw(drawable, states);
}

void Camera::draw_energy_bar(sf::Vector2f screen_position, const EnergyBar& bar_specs,
                             const std::size_t max_val, int current_val)
{
  sf::RectangleShape rect;
  rect.setSize(bar_specs.size);
  rect.setOutlineColor(sf::Color::Black);
  rect.setOutlineThickness(1);
  rect.setFillColor({25, 25, 25, 200});
  screen_position.x -= bar_specs.size.x/2;
  rect.setPosition(screen_position);

  this->draw(rect);

  rect.setOutlineThickness(1);
  float grad_width = bar_specs.size.x / (max_val / bar_specs.little_graduation);

  sf::Color color = mix(bar_specs.min_color, bar_specs.max_color,
                        static_cast<float>(current_val) / max_val);

  rect.setSize({grad_width, bar_specs.size.y});
  while (current_val > 0)
    {
      if (current_val >= bar_specs.little_graduation)
        rect.setFillColor(color);
      else
        rect.setFillColor(color * sf::Color(255, 255, 255, 100));
      rect.setPosition(screen_position);
      this->draw(rect);

      current_val -= bar_specs.little_graduation;
      screen_position.x += grad_width;
    }
}

void Camera::on_new_entity(const Entity* entity)
{
  this->sprites.push_back(std::make_unique<PicpicSprite>(entity));
}

const sf::Vector2u Camera::get_win_size() const
{
  return this->win().getSize();
}

void Camera::graphical_tick()
{
  for (auto& sprite: this->sprites)
    sprite->tick();
  this->fog.invalidate();
}

const GameClient* Camera::get_game_client() const
{
  return this->game;
}
