#include <logging/logging.hpp>
#include <assets/assets_manager.hpp>
#include <gui/camera/camera.hpp>
#include <gui/screen/screen.hpp>
#include <gui/sprites/pic_sprite.hpp>
#include <gui/sprites/orange_blob.hpp>
#include <gui/sprites/tourbillon_sprite.hpp>
#include <gui/sprites/bullet_sprite.hpp>
#include <gui/sprites/emp_sprite.hpp>
#include <gui/effect.hpp>
#include <world/world.hpp>
#include <world/layer.hpp>
#include <world/entity.hpp>
#include <world/team.hpp>
#include <world/task.hpp>
#include <gui/utils.hpp>
#include <game/game_client.hpp>
#include <climits>
#include <cstdlib>
#include <gui/effects/explosion.hpp>
#include <gui/camera/mobility_sprites.hpp>

#include <world/works/path_work.hpp>

#include <gui/sprites/decoration_sprite.hpp>

#include <world/abilities.hpp>
#include <world/location.hpp>
#include <world/health.hpp>

#include <algorithm>
#include <iostream>

using namespace std::string_literals;

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

  if (!ParticleEffect::texture.loadFromFile(AssetsManager::full_name("images/particles.png")))
    {
      log_debug("Failed to load particle.png");
      exit(1);
    }

  this->decoration_sprites.push_back(std::make_unique<DecorationSprite<49>>(Position{800, 800},
                           AssetsManager::full_name("images/test/tree.png"),
                           sf::Vector2<int>{256, 400}, 35ms));
  this->decoration_sprites.push_back(std::make_unique<DecorationSprite<49>>(Position{900, 850},
                           AssetsManager::full_name("images/test/tree.png"),
                           sf::Vector2<int>{256, 400}, 35ms));

  MobilitySprites::init();
  OrangeBlob::init();
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
  const Position pos = this->screen_to_world_position(event.mouseButton.x,
                                                      event.mouseButton.y);
  bool queue = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
  log_debug("queue: " << queue);

  // A right click when there's an action associated with the left click
  // resets the default action of the right click.
  if (this->screen->get_left_click().callback)
    this->screen->reset_left_click();

  // else // otherwise it always does the move action.
    {
      std::vector<EntityId> ids;
      for (const auto& entity: this->game->get_selection().get_entities())
        ids.push_back(entity->get_id());
      if (ids.empty())
        return ;
      const Entity* entity_under_mouse = this->get_entity_under_mouse();
      if (entity_under_mouse && std::find(ids.begin(), ids.end(), entity_under_mouse->get_id()) == ids.end())
        this->game->action_follow(ids, entity_under_mouse->get_id(), queue);
      else
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
      const Position pos = this->screen_to_world_position(event.mouseButton.x,
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
  this->mouse_selection.end();
}

void Camera::set_mouse_selection_to_selection()
{
  sf::Vector2i mouse_pos = this->get_mouse_position();

  // Only the manipulable entities are considered
  std::vector<const Entity*> entities_in_mouse_selection;

  const auto sorted_entities = this->world().sorted_entities();

  for (const auto& entity: sorted_entities)
    {
      Location* location = entity->get<Location>();
      Team* team = entity->get<Team>();
      if (!team || !location || !entity->is_manipulable())
        continue;
      if (this->mouse_selection.contains(mouse_pos,
                                         this->world_to_camera_position(location->position())))
        {
          entities_in_mouse_selection.push_back(entity);
          if (!this->mouse_selection.is_rectangular_selection_ongoing(mouse_pos))
            // Only select one entity, because we are not doing a
            // rectangular selection.
            break;
        }
    }

  if (entities_in_mouse_selection.empty())
    return ;

  auto end_own_entities = std::partition(entities_in_mouse_selection.begin(),
                                         entities_in_mouse_selection.end(),
                                         [this](const Entity* entity) -> bool
                                         {
                                           Team* team = entity->get<Team>();
                                           if (team->get() == this->game->get_self_team())
                                             return true;
                                           return false;
                                         });
  if (end_own_entities == entities_in_mouse_selection.begin())
    { // 0 of the selected entities are the player's
      this->game->get_selection().assign(entities_in_mouse_selection.begin(),
                                         entities_in_mouse_selection.end());
    }
  else
    { // Only select entities owned by the player
      this->game->get_selection().assign(entities_in_mouse_selection.begin(),
                                         end_own_entities);
    }
}

void Camera::add_mouse_selection_to_selection()
{
  sf::Vector2i mouse_pos = this->get_mouse_position();

  for (const auto& entity: this->world().entities)
    {
      Location* location = entity->get<Location>();
      if (!location || !entity->is_manipulable())
        continue;
      if (this->mouse_selection.contains(mouse_pos,
                                         this->world_to_camera_position(location->position())))
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
  if (this->get_game_client()->get_selection().is_empty())
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
    for (auto& effect: this->effects)
      effect->update(dt);
    for (auto& sprite: this->sprites)
      sprite->update(dt);
    for (auto& sprite: this->decoration_sprites)
      sprite->update(dt);

    this->effects.erase(std::remove_if(this->effects.begin(), this->effects.end(),
                                       [](const std::unique_ptr<Effect>& effect)
                                       {
                                         return effect->current_life() <= 0s;
                                       }),
                        this->effects.end());
}

void Camera::center(const sf::Vector2i& center)
{
  auto size = this->win().getSize();
  size.x /= 2;
  size.y /= 2;
  log_debug("width :"  << size.x);
  log_debug("height:"  << size.y);
  if (size.x < center.x)
    this->x = center.x - size.x;
  else
    this->x = 0;

  if (size.y < center.y)
    this->y = center.y - size.y;
  else
    this->y = 0;
}

bool Camera::tile_before_sprite(const CellIndex cell, const WorldSprite* sprite,
                                const Cell sprite_cell) const
{
  unsigned short x, y;
  std::tie(x, y) = this->map().index_to_cell(cell);
  unsigned short sprite_x, sprite_y;
  std::tie(sprite_x, sprite_y) = sprite_cell;
  CellIndex sprite_cell_index = this->map().cell_to_index(std::make_pair(sprite_x, sprite_y));
  // Same tile
  if (sprite_cell_index == cell)
    return true;
  if (sprite_y > y)
    return true;
  return this->map().is_cell_in_neighbour_lower_cells(sprite_cell_index, cell);
void Camera::draw()
{
  const sf::Vector2i mouse_pos = this->get_mouse_position();
  const Entity* entity_under_mouse = this->get_entity_under_mouse();

  std::vector<const WorldSprite*> sorted_sprites;
  sorted_sprites.reserve(this->sprites.size() +
                         this->effects.size() +
                         this->decoration_sprites.size());

  for (auto& sprite: this->sprites)
    {
      if (this->world().can_be_seen_by_team(sprite->get_world_pos(),
                                            this->game->get_self_team()))
        sorted_sprites.push_back(sprite.get());
    }
  for (auto& effect: this->effects)
    sorted_sprites.push_back(effect.get());
  for (auto& sprite: this->decoration_sprites)
    sorted_sprites.push_back(sprite.get());

  std::vector<std::unique_ptr<SimpleWorldSprite>> flag_sprites;

  for (const std::shared_ptr<Entity>& entity: this->world().entities)
    {
      const PathWork* path_work = entity->get_current_work<PathWork>();
      if (!path_work)
        continue;
      const PathTask* path_task = path_work->get_task<PathTask>();
      if (!path_task)
        continue;
      const Path& path = path_task->get_path();
      MobilitySprites::insert_flag_sprites(path, flag_sprites);
    }

  for (const std::unique_ptr<SimpleWorldSprite>& flag_sprite: flag_sprites)
    sorted_sprites.push_back(flag_sprite.get());

  std::sort(sorted_sprites.begin(),
            sorted_sprites.end(),
            [](const WorldSprite* a, const WorldSprite* b)
            {
              return a->get_world_pos().y < b->get_world_pos().y;
            });

  std::vector<Cell> sprite_cells;
  sprite_cells.reserve(this->sprites.size() + this->effects.size());
  for (const auto& sprite: sorted_sprites)
    sprite_cells.push_back(this->world().get_cell_at_position(sprite->get_world_pos()));

  std::vector<std::vector<CellIndex>> tiles_before_sprite;
  // We have one more vector, in which we put cells that are to be drawn
  // after all the tiles
  tiles_before_sprite.resize(sorted_sprites.size() + 1);

  unsigned short start_x = std::max(0, static_cast<int>((this->x / TILE_WIDTH)) - 1);
  unsigned short start_y = std::max(0, static_cast<int>(this->y / (TILE_HEIGHT / 2)) - 3);
  for (auto y = start_y; y < start_y + 30; y++)
    for (auto x = start_x; x < start_x + 25; x++)
      {
        CellIndex cell = this->map().cell_to_index(std::make_pair(x, y));
        for (std::size_t i = 0; i < sorted_sprites.size(); i++)
          {
            const WorldSprite* sprite = sorted_sprites[i];
            const Cell sprite_cell = sprite_cells[i];
            if (this->tile_before_sprite(cell, sprite, sprite_cell))
              {
                tiles_before_sprite[i].push_back(cell);
                goto break_outerloop;
              }
          }
        // We con only get here if the cell was added in no vector at all
        tiles_before_sprite[tiles_before_sprite.size()-1].push_back(cell);
      break_outerloop:
        continue;
      }

  for (std::size_t i = 0; i < tiles_before_sprite.size(); ++i)
    {
      for (const CellIndex index: tiles_before_sprite[i])
        {
          unsigned short col;
          unsigned short row;
          std::tie(col, row) = this->map().index_to_cell(index);
          auto x = col * TILE_WIDTH;
          if (row % 2 != 0)
            x += HALF_TILE_W;
          auto y = row * (TILE_HEIGHT / 2);
          for (Layer* layer: this->map().layers)
            {
              if (layer && layer->cells)
                {
                  std::size_t gid = layer->cells[index];
                  GraphTile* tile = this->tileset.tiles[gid].get();
                  if (tile)
                    {
                      if (std::find(this->world().current_path.begin(),
                                    this->world().current_path.end(),
                                    index) != this->world().current_path.end())
                        tile->sprite.setColor(sf::Color::Red);
                      else
                        tile->sprite.setColor(sf::Color::White);
                      tile->sprite.setPosition(x - this->x, y - this->y);
                      this->win().draw(tile->sprite);
                    }
                }
              y -= LEVEL_HEIGHT;
            }
        }
      // Draw the WorldSprite that is above all these tiles
      if (i < sorted_sprites.size())
        { // Unless we were at the last queue of tiles, which have no tile above them
          const WorldSprite* sprite = sorted_sprites[i];
          auto entity_sprite = dynamic_cast<const EntitySprite*>(sprite);
          const auto camera_pos = this->world_to_camera_position(sprite->get_world_pos());
          sf::Transform transform;

          if (entity_sprite &&
              entity_sprite->get_entity()->is_manipulable() &&
              (this->mouse_selection.contains(this->get_mouse_position(), camera_pos)))
            this->draw_hover_indicator(camera_pos, 80);

          if (entity_sprite &&
              this->get_game_client()->is_entity_selected(entity_sprite->get_entity()))
            this->draw_selected_indicator(camera_pos, 80);

          transform.translate(camera_pos.x - this->x,
                              camera_pos.y - this->y);
          sprite->draw(this->win(), transform);
          if (entity_sprite)
            {
              auto entity = entity_sprite->get_entity();
              auto health = entity->get<Health>();
              auto team = entity->get<Team>();

              if (health)
                {
                  auto bar = WorldSprite::standard_health_bar;
                  if (team && team->get() == 1)
                    bar.max_color = sf::Color::Blue;
                  this->draw_energy_bar(bar, health->get_max().to_int(),
                                        health->get().to_int(), transform);
                }
            }
        }
    }

  this->draw_mouse_selection();
  this->draw(this->fog.get_sprite());

  // Debug
  const std::string cam_pos = "Camera position: " + std::to_string(this->x) + ", " + std::to_string(this->y);
  this->game->get_debug_hud().add_debug_line(cam_pos);
  this->game->get_debug_hud().add_debug_line("Map width in pixel: " + std::to_string(this->map().get_width_in_pixels()) + ", " + std::to_string(this->map().get_height_in_pixels()));

  Position world_mouse_pos = this->camera_to_world_position(mouse_pos.x, mouse_pos.y);
  this->game->get_debug_hud().add_debug_line("Mouse world position: " +
                                             std::to_string(world_mouse_pos.x.to_double()) + ", " +
                                             std::to_string(world_mouse_pos.y.to_double()));

  if (entity_under_mouse)
    this->game->get_debug_hud().add_debug_line("Entity under mouse: " + std::to_string(entity_under_mouse->get_id()));
  else
    this->game->get_debug_hud().add_debug_line("No entity under mouse");

  this->game->get_debug_hud().add_debug_line("Current world time: "s + std::to_string(this->get_game_client()->current_world_time().count()));

  Cell cell_under_mouse = this->world().get_cell_at_position(world_mouse_pos);
  this->game->get_debug_hud().add_debug_line("Cell under mouse: "s + std::to_string(std::get<0>(cell_under_mouse)) + ":" + std::to_string(std::get<1>(cell_under_mouse)));
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

sf::Vector2i Camera::world_to_screen_position(const Position& pos) const
{
  auto res = this->world_to_camera_position(pos);
  res.x -= this->x;
  res.y -= this->y;
  return res;
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
  return this->screen_to_world_position(x - this->x, y - this->y);
}

Position Camera::screen_to_world_position(const int x, const int y) const
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

const Entity* Camera::get_entity_under_mouse() const
{
  auto mouse_pos = this->get_mouse_position();
  for (auto it = this->sprites.crbegin(); it != this->sprites.crend(); ++it)
    {
      EntitySprite* sprite = it->get();
      if (this->is_mouse_over(sprite) == true)
        return sprite->get_entity();
    }
  return nullptr;
}

bool Camera::is_mouse_over(const EntitySprite* sprite) const
{
  auto location = sprite->get_entity()->get<Location>();
  if (!location)
    return false;

  const auto pos = this->get_mouse_position();
  Position mouse_pos = this->camera_to_world_position(pos.x,
                                                        pos.y);
  Position ent_pos = location->position();

  // TODO, use different values depending on the sprite's size
  return (mouse_pos.x > ent_pos.x - 50 &&
          mouse_pos.x < ent_pos.x + 50 &&
          mouse_pos.y > ent_pos.y - 80 &&
          mouse_pos.y < ent_pos.y + 20);
}

void Camera::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
  this->win().draw(drawable, states);
}

void Camera::draw_energy_bar(const EnergyBar& bar_specs, const std::size_t max_val,
                             int current_val, const sf::RenderStates& states)
{
  sf::RectangleShape rect;
  rect.setSize(bar_specs.size);
  rect.setOrigin(bar_specs.size.x/2, bar_specs.size.y/2);
  rect.setOutlineColor(sf::Color::Black);
  rect.setOutlineThickness(1);
  rect.setFillColor({25, 25, 25, 200});
  rect.setPosition(0, -70);

  this->draw(rect, states);

  rect.setOutlineThickness(1);
  float grad_width = bar_specs.size.x / (max_val / bar_specs.little_graduation);

  sf::Color color = mix(bar_specs.min_color, bar_specs.max_color,
                        0.3 + (static_cast<float>(current_val) / max_val) * 0.7);

  rect.setSize({grad_width, bar_specs.size.y});
  while (current_val > 0)
    {
      if (current_val >= bar_specs.little_graduation)
        rect.setFillColor(color);
      else
        rect.setFillColor(color * sf::Color(255, 255, 255, 100));
      this->draw(rect, states);
      rect.move(grad_width, 0);

      current_val -= bar_specs.little_graduation;
    }
}

void Camera::draw_vertical_bar(sf::Vector2f screen_position, const EnergyBar& bar_specs,
                               const std::size_t max_val, int current_val)
{
  sf::RectangleShape rect;
  rect.setSize(bar_specs.size);
  rect.setOutlineColor(sf::Color::Black);
  rect.setOutlineThickness(1);
  rect.setFillColor({25, 25, 25, 200});
  screen_position.x -= bar_specs.size.x/2;
  screen_position.y -= bar_specs.size.y/2;
  rect.setPosition(screen_position);

  float grad_width = bar_specs.size.x / (max_val / bar_specs.little_graduation);

  this->draw(rect);

  rect.setOutlineThickness(1);
  float grad_height = bar_specs.size.y / (max_val / bar_specs.little_graduation);

  sf::Color color = mix(bar_specs.min_color, bar_specs.max_color,
                        static_cast<float>(current_val) / max_val);

  rect.setSize({bar_specs.size.x, grad_height});
  while (current_val > 0)
    {
      if (current_val >= bar_specs.little_graduation)
        rect.setFillColor(color);
      else
        rect.setFillColor(color * sf::Color(255, 255, 255, 100));
      rect.setPosition(screen_position);
      this->draw(rect);

      current_val -= bar_specs.little_graduation;
      screen_position.y += grad_height;
    }
}

void Camera::on_new_entity(const Entity* entity)
{
  if (entity->get_type() == 0)
    this->sprites.push_back(std::make_unique<PicpicSprite>(entity));
  else if (entity->get_type() == 1)
    this->sprites.push_back(std::make_unique<BulletSprite>(entity));
  else if (entity->get_type() == 2)
    this->sprites.push_back(std::make_unique<EmpSprite>(entity));
  else if (entity->get_type() == 4)
    this->sprites.push_back(std::make_unique<OrangeBlob>(entity));

}

void Camera::on_entity_deleted(const Entity* entity)
{
  // Look for an EntitySprite using this entity pointer, and remove it
  auto it = std::find_if(this->sprites.begin(),
                         this->sprites.end(),
                         [entity](const auto& s)
                         {
                           return s->get_entity() == entity;
                         });
  if (it != this->sprites.end())
    {
      log_debug("removing sprite.");
      this->sprites.erase(it);
    }
}

void Camera::on_entity_task_changed(const Entity* entity, const Task* task)
{
  // Look for an EntitySprite using this entity pointer
  auto it = std::find_if(this->sprites.begin(),
                         this->sprites.end(),
                         [entity](const auto& s)
                         {
                           return s->get_entity() == entity;
                         });
  assert(it != this->sprites.end());
  (*it)->set_task(task);
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

void Camera::add_effect(std::unique_ptr<Effect>&& effect)
{
  this->effects.push_back(std::move(effect));
}
