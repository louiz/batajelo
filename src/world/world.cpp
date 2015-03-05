#include <logging/logging.hpp>
#include <world/world.hpp>
#include <world/path.hpp>
#include <world/work.hpp>
#include <world/health.hpp>
#include <world/team.hpp>
#include <world/works/path_work.hpp>
#include <world/works/follow_work.hpp>
#include <world/location.hpp>
#include <world/team.hpp>
#include <world/vision.hpp>
#include <world/abilities.hpp>
#include <world/world_callbacks.hpp>
#include <utils/time.hpp>
#include <algorithm>
#include <iterator>

World::World():
  callbacks(std::make_unique<WorldCallbacks>())
{
  this->map.load_from_file("test6.tmx");
}

World::~World()
{
}

void World::insert_entity(std::unique_ptr<Entity>&& entity)
{
  this->new_entities.emplace_back(std::move(entity));
}

void World::tick()
{
  for (const auto& entity: this->entities)
    entity->tick(this);
  // Remove entities that were killed
  for (const auto& entity: this->entities)
    {
      if (entity->is_dead())
        this->callbacks->entity_deleted(entity.get());
    }
  this->entities.erase(std::remove_if(this->entities.begin(), this->entities.end(),
                                      [](const std::shared_ptr<Entity>& entity) -> bool
                                      {
                                        return entity->is_dead();
                                      }),
                       this->entities.end());

  // Insert new entities
  if (!this->new_entities.empty())
    {
      log_debug("Inserting " << this->new_entities.size() << " entities");
      log_debug("Before: " << this->entities.size());
      this->entities.reserve(this->entities.size() + this->new_entities.size());
      for (std::unique_ptr<Entity>& new_entity: this->new_entities)
        this->entities.emplace_back(new_entity.release());
      this->new_entities.clear();
      // std::move(this->new_entities.begin(), this->new_entities.end(),
      //           std::back_inserter(this->entities));
      log_debug("After: " << this->entities.size());
    }

  // Mark all entity with a Health == 0 as dead
  for (const auto& entity: this->entities)
    {
      Health* health = entity->get<Health>();
      if (health && health->get() == 0)
        entity->kill();
    }

  // Regroup all entities marked as dead at the begining of the vector
  auto end_dead = std::partition(this->entities.begin(), this->entities.end(),
                                 [](const std::shared_ptr<Entity>& entity)
                                 {
    return entity->is_dead();
  });

  // On each entity marked as dead, call entity_deleted()
  std::for_each(this->entities.begin(), end_dead,
               [this](const std::shared_ptr<Entity>& entity)
               {
                 this->callbacks->entity_deleted(entity.get());
               });
  // and remove from the list of world entities. Note that the entity shared
  // pointer may still be alive somewhere else, so the object may not be
  // destroyed here.
  this->entities.erase(this->entities.begin(), end_dead);
}

void World::seed()
{
  this->rng.seed();
}

unsigned long World::get_seed() const
{
  return this->rng.get_seed();
}

void World::seed(unsigned long s)
{
  this->rng.seed(s);
}

Path World::calculate_path(Position endpos, Location* location)
{
  log_debug("Calculating path for entity. Starting at position: " << location->position() << " ending at pos: " << endpos);

  CellPath cell_path = this->map.do_astar(this->get_cell_at_position(location->position()),
                                          this->get_cell_at_position(endpos));

  // DEBUG
  this->current_path = cell_path;

  if (!cell_path.empty())
    return this->smooth_path(cell_path, location->position(), endpos, location->get_width());
  return {};
}

Entity* World::do_new_entity(const EntityType type, const Position& pos, const uint16_t team_value)
{
  auto entity = this->entity_factory.make_entity(type);
  Location* location = entity->get<Location>();
  assert(location);
  location->position() = pos;
  Team* team = entity->get<Team>();
  assert(team);
  team->set(team_value);

  auto res = entity.get();
  this->insert_entity(std::move(entity));
  this->callbacks->entity_created(res);
  return res;
}

void World::do_move(const std::vector<EntityId>& ids, const Position& pos, const bool queue)
{
  Entity* entity;
  for (const EntityId id: ids)
    {
      log_debug("Moving entity " << id << " to pos " << pos);
      entity = this->get_entity_by_id(id);
      if (!entity)
        continue;
      if (queue)
        entity->queue_work(std::make_unique<PathWork>(entity, this, pos));
      else
        entity->set_work(std::make_unique<PathWork>(entity, this, pos));
    }
}

void World::do_follow(const std::vector<EntityId>& ids, const EntityId& target_id, const bool queue)
{
  Entity* entity;
  std::shared_ptr<Entity> target = this->get_shared_entity_by_id(target_id);
  if (!target)
    {
      log_debug("Could not follow entity " << target_id << ", it does not exist");
      return;
    }

  for (const EntityId id: ids)
    {
      log_debug("Entity " << id << " follows entity " << target_id);
      entity = this->get_entity_by_id(id);
      if (!entity)
        continue;
      if (queue)
        entity->queue_work(std::make_unique<FollowWork>(entity, this, target));
      else
        entity->set_work(std::make_unique<FollowWork>(entity, this, target));
    }
}

void World::do_cast_on_target(const std::vector<EntityId>& ids, const EntityId target_id, const AbilityType& type, const bool queue)
{
  std::shared_ptr<Entity> target = this->get_shared_entity_by_id(target_id);
  if (!target)
    {
      log_warning("Could not cast ability on target " << target_id);
      return ;
    }
  for (const EntityId id: ids)
    {
      Entity* entity = this->get_entity_by_id(id);
      if (!entity)
        continue;
      Ability* ability = get_ability(entity, type);
      if (ability)
        ability->cast(entity, this, target, queue);
      else
        {
          log_warning("Received a cast order, but entity " << id << " does not have ability: " << static_cast<int>(type));
        }
    }
}

void World::do_cast_on_pos(const std::vector<EntityId>& ids, const Position& pos, const AbilityType& type, const bool queue)
{
  for (const EntityId id: ids)
    {
      Entity* entity = this->get_entity_by_id(id);
      if (!entity)
        continue;
      Ability* ability = get_ability(entity, type);
      if (ability)
        ability->cast(entity, this, pos, queue);
      else
        {
          log_warning("Received a cast order, but entity " << id << " does not have ability: " << static_cast<int>(type));
        }
    }
}

std::shared_ptr<Entity> World::get_shared_entity_by_id(EntityId id)
{
  // Should use something like this->entities[id], to optimize.
  // Entities should be placed directly in a vector, for fast access.
  for (const auto& entity: this->entities)
    {
      if (entity->get_id() == id)
        return entity;
    }
  return {};
}

Entity* World::get_entity_by_id(EntityId id)
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

Map& World::get_map()
{
  return this->map;
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
      std::tie(x, y) = this->get_cell_at_position(pointer);
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

bool World::can_be_seen_by_team(const Position& position, const uint16_t team)
{
  auto res =
      std::find_if(this->entities.begin(), this->entities.end(),
                   [&position, team](const std::shared_ptr<Entity>& entity)
                   {
        Team* entity_team = entity->get<Team>();
        if (!entity_team || (entity_team->get() != team))
          return false;
        Location* entity_location = entity->get<Location>();
        Vision* entity_vision = entity->get<Vision>();
        if (!entity_vision || !entity_location)
          return false;
        Fix16 distance =
            Position::distance(position, entity_location->position());
        if (distance > entity_vision->get_range())
          return false;
        return true;
      });
  return res != this->entities.end();
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
  const unsigned int x = cell % this->map.get_width_in_tiles();
  const unsigned int y = cell / this->map.get_width_in_tiles();
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
