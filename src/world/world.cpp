#include <logging/logging.hpp>
#include <world/world.hpp>
#include <world/path.hpp>
#include <world/work.hpp>

World::World()
{
  this->map.load_from_file("test6.tmx");
}

World::~World()
{
}

void World::insert_unit(std::unique_ptr<Unit>&& unit)
{
  log_debug("Inserting unit at position " << unit->pos);
  this->units.push_back(unit.get());
  this->insert_entity(std::move(unit));
}

void World::insert_building(std::unique_ptr<Building>&& building)
{
  this->buildings.push_back(building.get());
  this->insert_entity(std::move(building));
}

void World::insert_entity(std::unique_ptr<Entity>&& entity)
{
  this->entities.push_back(std::move(entity));
}

void World::tick()
{
  for (const auto& entity: this->entities)
    entity->tick(this);
}

Path World::calculate_path(Position endpos, Unit* unit)
{
  log_debug("Calculating path for unit. Starting at position: " << unit->pos << " ending at pos: " << endpos);

  CellPath cell_path = this->map.do_astar(this->get_cell_at_position(unit->pos),
                                          this->get_cell_at_position(endpos));

  // DEBUG
  this->current_path = cell_path;

  if (!cell_path.empty())
    return this->smooth_path(cell_path, unit->pos, endpos, unit->width);
  return {};
}

Unit* World::do_new_unit(const EntityType type, const Position& pos)
{
  auto entity = this->entity_factory.make_unit(type);
  entity->pos = pos;
  auto res = entity.get();
  this->insert_unit(std::move(entity));
  return res;
}

void World::do_move(const std::vector<EntityId> ids, const Position& pos, const bool queue)
{
  Unit* unit;
  for (const EntityId id: ids)
    {
      log_debug("Moving unit " << id << " to pos " << pos);
      unit = this->get_unit_by_id(id);
      if (queue)
        unit->queue_work(std::make_unique<PathWork>(unit, pos));
      else
        unit->set_work(std::make_unique<PathWork>(unit, pos));
    }
}

Entity* World::get_entity_by_id(unsigned short id)
{
  // Should use something like this->entities[id], to optimize.
  // Entities should be placed directly in a vector, for fast access.
  for (const auto& entity: this->entities)
    {
      if (entity->get_id() == id)
        return entity.get();
    }
  return nullptr;
}

Unit* World::get_unit_by_id(unsigned short id)
{
  Unit* unit;
  for (std::list<Unit*>::iterator it = this->units.begin(); it != this->units.end(); ++it)
    {
      unit = *it;
      if (unit->get_id() == id)
     return unit;
    }
  return 0;
}

Building* World::get_building_by_id(unsigned short id)
{
  Building* building;
  for (std::list<Building*>::iterator it = this->buildings.begin(); it != this->buildings.end(); ++it)
    {
      building = *it;
      if (building->get_id() == id)
     return building;
    }
  return 0;
}

Map& World::get_map()
{
  return this->map;
}

static bool compare_units(const Unit* a, const Unit* b)
{
  return (a->pos.y < b->pos.y);
}

Cell World::get_cell_at_position(const Position& pos) const
{
  unsigned short x =  std::floor(pos.x / CELL_SIZE).to_int();
  unsigned short y =  std::floor(pos.y / CELL_SIZE).to_int() * 2;

  const Fix16 rel_x = pos.x % CELL_SIZE;
  const Fix16 rel_y = pos.y % CELL_SIZE;

  if (rel_x + rel_y < HALF_CELL_SIZE)
    { // top left
      if (x == 0 || y == 0)
        return InvalidCell;
      x--;
      y--;
    }
  else if ((rel_x >= HALF_CELL_SIZE) && (rel_y < HALF_CELL_SIZE) &&
           (rel_x - HALF_CELL_SIZE >= rel_y))
    { // top right
      if (y == 0)
        return InvalidCell;
      y--;
    }
  else if ((rel_x < HALF_CELL_SIZE) && (rel_y >= HALF_CELL_SIZE) &&
           (rel_y - HALF_CELL_SIZE > rel_x))
    { // bottom left
      if (x == 0 || y == this->map.get_width_in_tiles())
        return InvalidCell;
      x--;
      y++;
    }
  else if ((rel_x >= HALF_CELL_SIZE) && (rel_y >= HALF_CELL_SIZE) &&
           (rel_x - HALF_CELL_SIZE + rel_y - HALF_CELL_SIZE > HALF_CELL_SIZE))
    { // bottom right
      if (y == this->map.get_width_in_tiles())
        return InvalidCell;
      y++;
    }

  return std::make_tuple(x, y);
}

Fix16 World::get_position_height(const Position& pos) const
{
  const int cell_size = static_cast<const int>(CELL_SIZE);

  short cellx;
  short celly;
  std::tie(cellx, celly) = this->get_cell_at_position(pos);

  TileHeights heights = this->map.get_cell_heights(cellx, celly);

  Fix16 cx = (pos.x % (cell_size)) / cell_size;

  Fix16 cy = (pos.y % (cell_size)) / (cell_size / 2);

  cy /= 2;

  if (celly % 2)
    {
      cy += 0.5;
      cx += 0.5;
    }
  if (cx > 1)
    cx -= 1;
  if (cy > 1)
    cy -= 1;

  Fix16 left = heights.corners.left;
  Fix16 top = heights.corners.top;
  Fix16 right = heights.corners.right;
  Fix16 bot = heights.corners.bottom;

  Fix16 hx = left + (cx * (right - left));

  Fix16 hy = top + (cy * (bot - top));
  Fix16 hy2 = top + (0.5 * (bot - top));

  return hx + (hy - hy2);
}

Path World::smooth_path(CellPath path,
                        Position& start, const Position& end, const Fix16 width) const
{
  Path res;
  Position current_pos(start);
  Position next_pos;

  do
    {
      next_pos = this->get_next_path_position(path, current_pos, end, width);
      res.push_back(next_pos);
      current_pos = next_pos;
    } while (next_pos != end);

  return res;
}

Position World::get_next_path_position(CellPath& path,
                                       const Position& current, const Position& goal,
                                       const Fix16 unit_width) const
{
  const Fix16 step = 2;

  // First check if the whole path is just a simple straight line.
  if (this->can_walk_in_straight_line(current, goal, step, unit_width) == true)
    return goal;

  Position temp_goal(0, 0);
  Position prev_pos(current);

  bool first = true;

  while (true)
    {
      auto rit = path.rbegin();
      CellIndex cell = *rit;
      for (const auto& c: path)
        log_debug(this->map.index_to_cell(c));

      if (path.empty())
        return goal;

      if (path.size() == 1)
        temp_goal = goal;
      else
        {
          CellIndex next_cell = *(rit + 1);
          temp_goal = this->get_nearest_corner(current, cell, next_cell, unit_width);
        }

      if (!first && this->can_walk_in_straight_line(current, temp_goal,
                                                    step, unit_width) == false)
        return prev_pos;

      prev_pos = temp_goal;
      path.pop_back();
      first = false;
    }
}

bool World::can_walk_in_straight_line(const Position& start, const Position& end, const Fix16 step, const Fix16 width) const
{
  Vec2 forward(end - start);

  // TODO, use the real width value
  forward.set_length(width);

  Position start1 = start + forward.perpendicular1();
  Position end1 = end + forward.perpendicular1();
  Position start2 = start + forward.perpendicular2();
  Position end2 = end + forward.perpendicular2();

  log_error("can_walk_in_straight_line : start = " << start << ", end=" << end << " width=" << width);
  log_error("From " << start1 << " to " << end1 << ". And from " << start2 << " to " << end2);
  if (this->has_a_line_of_sight(start1, end1, step) == false)
    {
      log_debug("no1");
      return false;
    }
  if (this->has_a_line_of_sight(start2, end2, step) == false)
    {
      log_debug("no2");
      return false;
    }
  // if (this->has_a_line_of_sight(start, end, step) == false)
  //   {
  //     log_debug("no");
  //     return false;
  //   }
  log_debug("yes");
  return true;
}

bool World::has_a_line_of_sight(const Position& start, const Position& end,
                                const Fix16 step) const
{
  log_debug("has_a_line_of_sight: " << start << " and " << end);
  // The point to move along the line to check if everything is walkable.
  Position pointer(start);
  // A vector with a length of 'step' in the direction of the end position
  // We use this vector to move the pointor along the line.
  Vec2 forward(end - start);
  forward.set_length(step);

  log_debug("Forward: " << forward);

  // the cell position the pointer is at.
  unsigned short prevx, prevy;
  unsigned short x, y;
  std::tie(prevx, prevy) = this->get_cell_at_position(pointer);

  // Move the pointer until we reach the destination, or an obstacle.
  log_debug("Distance between start and end of path: "<< std::to_string(Position::distance(pointer, end)));
  while (Position::distance(pointer, end) >= step)
    {
      // Move the pointer forward.
      pointer += forward;
      log_debug("pointer is at "<< pointer);
      std::tie(x, y) = this->get_cell_at_position(pointer);
      log_debug("cell is: " << x << ":" << y);
      // If the cell changed, we check if we can walk from the previous to
      // the current one. If not, it's not walkable and we return false
      if ((prevx != x) || (prevy != y))
        {
          if (this->can_traverse_cell(prevx, prevy, x, y) == false)
            // we hit an obstabcle.
            {
              return false;
            }
        }
      prevy = y;
      prevx = x;
    }
  std::tie(x, y) = this->get_cell_at_position(end);
  if (((prevx != x) || (prevy != y)) && this->can_traverse_cell(prevx, prevy, x, y) == false)
    {
      return false;
    }
  return true;
}

bool World::can_traverse_cell(const short x, const short y,
                              const short x2, const short y2) const
{
  log_debug("can_traverse_cell: " << x << ":" << y << " " << x2 << ":" << y2);
  if (y == y2 && x == x2)
    {
      log_debug("same");
      return true;
    }

  // TODO in the case of a diagonal move, we currently return true.  Do a
  // more correct thing checking if (for example if we go right-down) we can
  // go right THEN down OR down THEN right. If one of those is possible,
  // return true, otherwise return false.
  TileHeights heights = this->map.get_cell_heights(x, y);
  TileHeights neighbour_heights = this->map.get_cell_heights(x2, y2);

  if ((y % 2 == 0 && y2 == y - 1 && x2 == x) ||
      (y % 2 == 1 && y2 == y - 1 && x2 == x + 1))
    { // move up-right
      log_debug("up right");
      if (heights.corners.top == neighbour_heights.corners.left &&
          heights.corners.right == neighbour_heights.corners.bottom)
        return true;
      else
        return false;
    }
  else if ((y % 2 == 0 && y2 == y - 1 && x2 == x - 1) ||
           (y % 2 == 1 && y2 == y - 1 && x2 == x))
    { // move up-left
      log_debug("up left");
      if (heights.corners.top == neighbour_heights.corners.right &&
          heights.corners.left == neighbour_heights.corners.bottom)
        return true;
      else
        return false;
    }
  else if ((y % 2 == 0 && y2 == y + 1 && x2 == x - 1) ||
           (y % 2 == 1 && y2 == y + 1 && x2 == x))
    { // bottom-left
      log_debug("bottom left");
      if (heights.corners.left == neighbour_heights.corners.top &&
          heights.corners.bottom == neighbour_heights.corners.right)
        return true;
      else
        return false;
    }
  else if ((y % 2 == 0 && y2 == y + 1 && x2 == x) ||
           (y % 2 == 1 && y2 == y + 1 && x2 == x + 1))
    { // bottom right
      log_debug("bottom right");
      if (heights.corners.right == neighbour_heights.corners.top &&
          heights.corners.bottom == neighbour_heights.corners.left)
        return true;
      else
        return false;
    }
  log_warning("diagonal");
  // We move in diagonal. TODO read the TODO above
  return false;
}

Position World::get_nearest_corner(const Position& pos,
                                   const CellIndex cell, const CellIndex next_cell,
                                   const Fix16 width) const
{
  log_debug("get_nearest_corner " << this->map.index_to_cell(cell) << " and " << this->map.index_to_cell(next_cell) << " width: " << width);
  unsigned short x;
  unsigned short y;
  std::tie(x, y) = this->map.index_to_cell(cell);

  unsigned short nx;
  unsigned short ny;
  std::tie(nx, ny) = this->map.index_to_cell(next_cell);

  log_debug("get_nearest_corner " << pos << " " << x << ":" << y << " && " << nx << ":" << ny);

  if ((y % 2 == 0 && ny == y - 1 && nx == x) ||
      (y % 2 == 1 && ny == y - 1 && nx == x + 1))
    {
      log_debug("top_right");
      // top right
      Position nearest_position((x + 1) * CELL_SIZE,
                                y * HALF_CELL_SIZE + width/2);
      Position other_position((x + 1) * CELL_SIZE + HALF_CELL_SIZE - width/2,
                              (y + 1) * HALF_CELL_SIZE);
      if (y % 2 == 0)
        {
          nearest_position.x -= HALF_CELL_SIZE;
          other_position.x -= HALF_CELL_SIZE;
        }

      auto shorter_distance = Position::distance(pos, nearest_position);
      auto other_distance = Position::distance(pos, other_position);
      log_debug("Two possible corners: " << nearest_position << " and " << other_position);
      if (other_distance < shorter_distance)
        return other_position;
      return nearest_position;
    }
  else if ((y % 2 == 0 && ny == y - 1 && nx == x - 1) ||
           (y % 2 == 1 && ny == y - 1 && nx == x))
    {
      log_debug("top_left");
      // up left
      Position nearest_position(x * CELL_SIZE + HALF_CELL_SIZE + width/2,
                                (y + 1) * HALF_CELL_SIZE);
      Position other_position((x + 1) * CELL_SIZE,
                              y * HALF_CELL_SIZE + width/2);
      log_debug("Two possible corners: " << nearest_position << " and " << other_position);
      if (y % 2 == 0)
        {
          nearest_position.x -= HALF_CELL_SIZE;
          other_position.x -= HALF_CELL_SIZE;
        }

      auto shorter_distance = Position::distance(pos, nearest_position);
      auto other_distance = Position::distance(pos, other_position);
      log_debug("Two possible corners: " << nearest_position << " and " << other_position);
      if (other_distance < shorter_distance)
        return other_position;
      return nearest_position;
    }
  else if ((y % 2 == 0 && ny == y + 1 && nx == x - 1) ||
           (y % 2 == 1 && ny == y + 1 && nx == x))
    {
      log_debug("bot_left");
      // bottom left
      Position nearest_position(x * CELL_SIZE + HALF_CELL_SIZE + width/2,
                                (y + 1) * HALF_CELL_SIZE);
      Position other_position((x + 1) * CELL_SIZE,
                              y * HALF_CELL_SIZE + CELL_SIZE - width/2);
      if (y % 2 == 0)
        {
          nearest_position.x -= HALF_CELL_SIZE;
          other_position.x -= HALF_CELL_SIZE;
        }

      auto shorter_distance = Position::distance(pos, nearest_position);
      auto other_distance = Position::distance(pos, other_position);
      log_debug("Two possible corners: " << nearest_position << " and " << other_position);
      if (other_distance < shorter_distance)
        return other_position;
      return nearest_position;
    }
  else if ((y % 2 == 0 && ny == y + 1 && nx == x) ||
           (y % 2 == 1 && ny == y + 1 && nx == x + 1))
    {
      log_debug("bot_right " << x << " " << y);
      // Bottom right
      Position nearest_position((x + 1) * CELL_SIZE,
                                (y + 2) * HALF_CELL_SIZE - width/2);
      Position other_position((x + 1) * CELL_SIZE + HALF_CELL_SIZE - width/2,
                              (y + 1) * HALF_CELL_SIZE);
      if (y % 2 == 0)
        {
          log_debug("subing HALF_CELL_SIZE");
          nearest_position.x -= HALF_CELL_SIZE;
          other_position.x -= HALF_CELL_SIZE;
        }

      auto shorter_distance = Position::distance(pos, nearest_position);
      auto other_distance = Position::distance(pos, other_position);
      log_debug("Two possible corners     biiiite: " << nearest_position << " and " << other_position);
      if (other_distance < shorter_distance)
        return other_position;
      return nearest_position;

    }
  assert(false);
}

Position World::get_nearest_corner(const Position& pos, const CellIndex cell, const Fix16 width) const
{
  log_debug("get_nearest_corner of final Cell. In cell " << this->map.index_to_cell(cell) <<
            " find the nearest corner of pos " << pos);
  const uint x = cell % this->map.get_width_in_tiles();
  const uint y = cell / this->map.get_width_in_tiles();
  Position nearest_position(x * 100 + width, y * 100 + width);
  Fix16 shorter_distance = Position::distance(pos, nearest_position);
  Position other_position(x * 100 + width, (y + 1) * 100 - width);
  Fix16 other_distance = Position::distance(pos, other_position);
  if (other_distance < shorter_distance)
    {
      shorter_distance = other_distance;
      nearest_position = other_position;
    }
  other_position = Position((x + 1) * 100 - width, y * 100 + width);
  other_distance = Position::distance(pos, other_position);
  if (other_distance < shorter_distance)
    {
      shorter_distance = other_distance;
      nearest_position = other_position;
    }
  other_position = Position((x + 1) * 100 - width, (y + 1) * 100 - width);
  other_distance = Position::distance(pos, other_position);
  if (other_distance < shorter_distance)
    {
      shorter_distance = other_distance;
      nearest_position = other_position;
    }
  return nearest_position;
}

bool World::can_build_at_cell(const int x, const int y) const
{
  return this->map.can_be_built_on(x, y);
}

bool World::can_unit_spawn_at_pos(const Unit* unit, const Position& pos) const
{
  for (const auto& entity: this->entities)
    {
      if (entity->is_obstructing_position(unit, pos))
        return false;
    }
  return true;
}
