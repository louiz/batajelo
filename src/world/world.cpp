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

  if (this->can_walk_in_straight_line(location->position(), endpos, location->get_width()) == true)
    return {endpos};

  CellPath cell_path = this->map.do_astar(this->get_cell_at_position(location->position()),
                                          this->get_cell_at_position(endpos));

  // DEBUG
  this->current_path = cell_path;

  if (!cell_path.empty())
    return this->smooth_path(cell_path, location->position(), endpos, location->get_width());
  return {};
}

Path World::smooth_path(CellPath path,
                        Position& start, const Position& end, const Fix16 width) const
{
  Path res;
  Position current_position = start;

  CellPath::reverse_iterator next_cell = path.rbegin();

  while (next_cell != path.rend())
    {
      next_cell = this->get_farthest_walkable_cell(current_position, path, next_cell, width);
      current_position = this->cell_center(*next_cell);
      next_cell++;
      if (next_cell == path.rend())
        res.push_back(end);
      else
        res.push_back(current_position);
    }
  return res;
}

CellPath::reverse_iterator World::get_farthest_walkable_cell(const Position& pos,
                                                             const CellPath& path,
                                                             const CellPath::reverse_iterator& current_cell,
                                                             const Fix16 width) const
{
  auto res = current_cell;

  auto it = current_cell + 1;
  while (it != path.rend())
    {
      if (this->can_walk_in_straight_line(pos, this->cell_center(*it), width))
        res = it;
      it++;
    }
  return res;
}

Position World::cell_center(const CellIndex index) const
{
  Cell cell = this->map.index_to_cell(index);
  const auto x = std::get<0>(cell);
  const auto y = std::get<1>(cell);

  Position res{(x + 1) * CELL_SIZE,
      (y + 1) * HALF_CELL_SIZE};

  if (y % 2 == 0)
    res.x -= HALF_CELL_SIZE;
  return res;
}

bool World::can_walk_in_straight_line(const Position& start, const Position& end, const Fix16 width) const
{
  static const Fix16 step = 25;
  Vec2 forward(end - start);

  forward.set_length(width);

  Position pstart = start + forward.perpendicular1();
  Position pend = end + forward.perpendicular1();

  if (this->has_a_line_of_sight(pstart, pend, step) == false)
    return false;
  pstart = start + forward.perpendicular2();
  pend = end + forward.perpendicular2();
  if (this->has_a_line_of_sight(pstart, pend, step) == false)
    return false;
  return true;
}

bool World::has_a_line_of_sight(const Position& start, const Position& end,
                                const Fix16 step) const
{
  // The point to move along the line to check if everything is walkable.
  Position pointer(start);
  // A vector with a length of 'step' in the direction of the end position
  // We use this vector to move the pointer along the line.
  Vec2 forward(end - start);
  forward.set_length(step);

  // the cell position the pointer is at.
  unsigned short prevx, prevy;
  unsigned short x, y;
  std::tie(prevx, prevy) = this->get_cell_at_position(pointer);

  // Move the pointer until we reach the destination, or an obstacle.
  Fix16 travelled_distance = 0;
  const Fix16 distance_to_travel = Position::distance(start, end);
  while (travelled_distance < distance_to_travel)
    {
      // Move the pointer forward.
      pointer += forward;
      travelled_distance += step;
      std::tie(x, y) = this->get_cell_at_position(pointer);
      // If the cell changed, we check if we can walk from the previous to
      // the current one. If not, it's not walkable and we return false
      if ((prevx != x) || (prevy != y))
        {
          if (this->can_traverse_cell(prevx, prevy, x, y) == false)
            // we hit an obstabcle.
            return false;
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

Entity* World::do_new_entity(const EntityType type, const Position& pos, const uint16_t team_value)
{
  auto entity = this->entity_factory.make_entity(this, type);
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
        entity->queue_work(std::make_unique<PathWork>(this, entity, pos));
      else
        entity->set_work(std::make_unique<PathWork>(this, entity, pos));
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
        entity->queue_work(std::make_unique<FollowWork>(this, entity, target));
      else
        entity->set_work(std::make_unique<FollowWork>(this, entity, target));
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

void World::do_cast(const std::vector<EntityId>& ids, const AbilityType& type, const bool queue)
{
  for (const EntityId id: ids)
    {
      log_debug("do_cast: " << id);
      Entity* entity = this->get_entity_by_id(id);
      if (!entity)
        continue;
      log_debug("ok");
      Ability* ability = get_ability(entity, type);
      if (ability)
        ability->cast(entity, this, queue);
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
  if (y == y2 && x == x2)
    return true;

  TileHeights heights = this->map.get_cell_heights(x, y);
  TileHeights neighbour_heights = this->map.get_cell_heights(x2, y2);

  // move up-right
  if ((y % 2 == 0 && y2 == y - 1 && x2 == x) ||
      (y % 2 == 1 && y2 == y - 1 && x2 == x + 1))
    return (heights.corners.top == neighbour_heights.corners.left &&
            heights.corners.right == neighbour_heights.corners.bottom);
  // move up-left
  else if ((y % 2 == 0 && y2 == y - 1 && x2 == x - 1) ||
           (y % 2 == 1 && y2 == y - 1 && x2 == x))
    return (heights.corners.top == neighbour_heights.corners.right &&
            heights.corners.left == neighbour_heights.corners.bottom);
  // bottom-left
  else if ((y % 2 == 0 && y2 == y + 1 && x2 == x - 1) ||
           (y % 2 == 1 && y2 == y + 1 && x2 == x))
    return (heights.corners.left == neighbour_heights.corners.top &&
            heights.corners.bottom == neighbour_heights.corners.right);
  // bottom right
  else if ((y % 2 == 0 && y2 == y + 1 && x2 == x) ||
           (y % 2 == 1 && y2 == y + 1 && x2 == x + 1))
    return (heights.corners.right == neighbour_heights.corners.top &&
            heights.corners.bottom == neighbour_heights.corners.left);

  // Diagonal
  if ((x2 == x - 1) && (y2 == y))
    {
      if (y % 2 == 0)
        return this->can_traverse_cell(x, y, x - 1, y - 1) &&
               this->can_traverse_cell(x, y, x - 1, y + 1);
      else
        return this->can_traverse_cell(x, y, x, y - 1) &&
               this->can_traverse_cell(x, y, x, y + 1);
    }
  else if ((x2 == x) && (y2 == y - 2))
    {
      if (y % 2 == 0)
        return this->can_traverse_cell(x, y, x, y - 1) &&
               this->can_traverse_cell(x, y, x - 1, y - 1);
      else
        return this->can_traverse_cell(x, y, x + 1, y - 1) &&
               this->can_traverse_cell(x, y, x, y - 1);
    }
  else if ((x2 == x + 1) && (y2 == y))
    {
      if (y % 2 == 0)
        return this->can_traverse_cell(x, y, x, y - 1) &&
               this->can_traverse_cell(x, y, x, y + 1);
      else
        return this->can_traverse_cell(x, y, x + 1, y + 1) &&
               this->can_traverse_cell(x, y, x + 1, y - 1);
    }
  else if ((x2 == x) && (y2 == y + 2))
    {
      if (y % 2 == 0)
        return this->can_traverse_cell(x, y, x - 1, y - 1) &&
               this->can_traverse_cell(x, y, x, y - 1);
      else
        return this->can_traverse_cell(x, y, x + 1, y - 1) &&
               this->can_traverse_cell(x, y, x, y - 1);
    }

  // Cells aren't adjacent, the function call shouldn't have been made
  assert(false);
  return false;
}
