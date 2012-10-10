#include <world/world.hpp>
#include <world/path.hpp>

World::World(Mod& mod):
  started(false),
  map(0)
{
  this->replay = new Replay;
  this->turn_handler = new TurnHandler(this->replay);
  this->init(mod);
}

World::World(Map* map, Mod& mod):
  started(false),
  map(map)
{
  this->replay = new Replay;
  this->turn_handler = new TurnHandler(this->replay);
  this->init(mod);
}

World::~World()
{
  delete this->turn_handler;
  if (this->replay != 0)
    delete this->replay;
  this->reset_entity_iterator();
  Entity* entity;
  while ((entity = this->get_next_entity()) != 0)
    delete entity;
  std::vector<const Unit*>::iterator it;
  for (it = this->unit_models.begin(); it < this->unit_models.end(); ++it)
    delete (*it);
}

Entity* World::get_next_entity()
{
  if (this->entities_iterator == this->entities.end())
    {
      this->entities_iterator = this->entities.begin();
      return 0;
    }
  Entity* entity = *this->entities_iterator;
  ++this->entities_iterator;
  return entity;
}

Entity* World::get_next_entity(const int y)
{
  return this->get_next_entity();
  // TODO FIX THAT


  // if (this->entities_iterator == this->entities.end())
  //   {
  //     this->entities_iterator = this->entities.begin();
  //     return 0;
  //   }
  // Entity* entity = *this->entities_iterator;
  // if (entity->pos.y < Fix16(y))
  //   {
  //     ++this->entities_iterator;
  //     return entity;
  //   }
  // return 0;
}

void World::set_next_turn_callback(t_next_turn_callback callback)
{
  log_debug("set_next_turn_callback");
  this->turn_handler->set_next_turn_callback(callback);
}

void World::reset_entity_iterator()
{
  this->entities_iterator = this->entities.begin();
}

void World::insert_unit(Unit* unit)
{
  this->units.push_back(unit);
  this->insert_entity(unit);
}

void World::insert_entity(Entity* entity)
{
  this->entities.push_back(entity);
}

void World::remove_occupant(Occupant* occupant)
{
  std::vector<Occupant*>::iterator it;
  Occupant* occupant_to_remove;

  for (it = this->occupants.begin(); it < this->occupants.end(); ++it)
    {
      if ((*it)->number == occupant->number)
	{
	  occupant_to_remove = (*it);
	  this->occupants.erase(it);
	  delete occupant_to_remove;
	  return ;
	}
    }
  assert(false);
}

void World::add_new_occupant(Occupant* occupant)
{
  this->occupants.push_back(occupant);
  log_debug("Adding new occupant to the world:" << occupant->name << " " << occupant->number);
}

void World::init(Mod& mod)
{
  log_debug("Init world");
  // TODO, load these units from the Mod file.
  // const Unit* unit = new Unit;
  // this->entity_models.push_back(unit);
  this->unit_models = mod.get_unit_models();
  this->building_models = mod.get_building_models();
  log_debug("Done.");
}

Unit* World::create_unit(unsigned int type)
{
  const Unit* model = this->unit_models[type];
  Unit* new_entity = new Unit(*model);
  return new_entity;
}

Unit* World::create_unit(unsigned int type, const Unit& e)
{
  log_debug("Creating entity of type " << type);
  const Unit* model = this->unit_models[type];
  Unit* new_unit = new Unit(*model);
  new_unit->pos = e.pos;
  return new_unit;
}

void World::pause()
{
  this->turn_handler->pause();
}

void World::unpause()
{
  this->turn_handler->unpause();
}


void World::tick(bool force)
{
  if ((this->started == false) && (force == false))
    return ;
  this->turn_handler->tick(force);
  if (this->turn_handler->is_paused() == true)
    return ;
  Entity* entity;
  this->reset_entity_iterator();
  // log_error("World.tick()");
  while ((entity = this->get_next_entity()))
    {
      entity->tick(this);
    }
}

void World::advance_replay_until_paused()
{
  do
    {
      this->tick(true);
    } while (this->turn_handler->is_paused() == false);
}

void World::start()
{
  log_debug("start");
  if (this->started == true)
    return;
  if (this->map == 0)
    {
      this->map = new Map();
      this->map->load_from_file("test4.tmx");
    }
  this->started = true;
}

void World::do_path(ActionEvent* event)
{
  DoMoveEvent* path_event = static_cast<DoMoveEvent*>(event);
  // Path path(path_event->x, path_event->y);
  Position endpos(static_cast<short>(path_event->x), static_cast<short>(path_event->y));
  std::vector<unsigned short>::const_iterator actors_it;
  for (actors_it = path_event->actors_ids.begin(); actors_it < path_event->actors_ids.end(); ++actors_it)
    {
      Unit* entity = static_cast<Unit*>(this->get_entity_by_id((*actors_it)));
      assert(entity != 0);
      // entity->set_path(path);
      // log_error("Path finding from " << entity->pos << " to " << path_event->x << ":" << path_event->y);
      short startx;
      short starty;
      short endx;
      short endy;
      this->get_cell_at_position(entity->pos, startx, starty);
      this->get_cell_at_position(endpos, endx, endy);
      this->current_path = this->map->do_astar(startx, starty, endx, endy);
      // cell_path_t temp_path(this->current_path.front(), this->current_path.back());
      if (this->current_path.size() > 0)
        entity->path =
          this->smooth_path(this->current_path, entity->pos, endpos, entity->width);
      // log_error("ENTITY_POSITION: " << entity->pos);
      // log_error("PATH TO FOLLOW IS: ");
      // for (std::list<Position>::const_iterator it = entity->path.begin();
      //      it != entity->path.end();
      //      ++it)
      //   log_error(*it);
      // log_error("FIN");
    }
}

void World::do_new_unit(ActionEvent* event)
{
  log_debug("DO NEW UNIT");
  DoUnitEvent* unit_event = static_cast<DoUnitEvent*>(event);
  // This unit just contains the initial position of the unit, and it's
  // type_id because we don't need to pass all these informations, because
  // we already have all the possible unit types in our list
  // (unit_models).
  Unit* unit = unit_event->unit;
  // We use the type id to create an unit using the corresponding model,
  // and we pass the initial position of the unit as well.
  Unit* new_unit = this->create_unit(unit->type_id, *unit);
  delete unit;
  log_debug("New_Unit: " << new_unit->pos);
  this->insert_unit(new_unit);
}

void World::do_build(ActionEvent* event)
{
  log_info("do_build");
  DoBuildEvent* build_event = static_cast<DoBuildEvent*>(event);
}

void World::completely_validate_action(const unsigned int id)
{
  log_debug("Action " << id << " completely validated.");
  return this->turn_handler->completely_validate_action(id);
}

Entity* World::get_entity_by_id(unsigned short id)
{
  // Should use something like this->entities[id], to optimize.
  // Entities should be placed directly in a vector, for fast access.
  Entity* entity;
  this->reset_entity_iterator();
  while ((entity = this->get_next_entity()))
    {
      if (entity->get_id() == id)
	return entity;
    }
  return 0;
}

void World::validate_turn_completely(const unsigned int number)
{
  this->turn_handler->completely_validate_turn(number);
}

void World::generate_command(const char* name, const std::string& archive)
{
  Command* command = new Command;
  command->set_name(name);
  command->set_body(archive.data(), archive.length());
  this->commands_queue.push(command);
  log_debug(this->commands_queue.size());
}

Command* World::get_pending_command()
{
  if (this->commands_queue.empty())
    return 0;
  Command* command = this->commands_queue.front();
  this->commands_queue.pop();
  return command;
}

Replay* World::get_replay() const
{
  return this->replay;
}

TurnHandler* World::get_turn_handler() const
{
  return this->turn_handler;
}

unsigned int World::get_number_of_occupants() const
{
  return this->occupants.size();
}

static bool compare_units(const Unit* a, const Unit* b)
{
  return (a->pos.y < b->pos.y);
}

void World::sort_entities()
{
  // this->entities.sort(compare_units);
}

void World::get_cell_at_position(const Position& pos,
                                 short& xa, short& ya) const
{
  const int16_t cell_size = static_cast<const int16_t>(CELL_SIZE);
  assert(cell_size % 2 == 0);
  assert(cell_size > 1);
  xa = static_cast<int16_t>(pos.x) / cell_size;
  ya = static_cast<int16_t>(pos.y) / cell_size;
}

mpreal World::get_position_height(const Position& pos) const
{
  short cellx;
  short celly;
  const int cell_size = static_cast<const int>(CELL_SIZE);
  this->get_cell_at_position(pos, cellx, celly);
  ushort heights = this->map->get_cell_heights(cellx, celly);
  mpreal cx = mpreal(int16_t(pos.x) % cell_size) / mpreal(CELL_SIZE);
  mpreal cy = mpreal(int16_t(pos.y) % cell_size) / mpreal(CELL_SIZE);
  mpreal a = (heights) & 15;
  mpreal b = (heights >> 4) & 15;
  mpreal d = (heights >> 12) & 15;
  mpreal hx = a - (cx * (a - b));
  mpreal hy = a - (a - (cy * (a - d)));
  return hx - hy;
}

bool World::is_started() const
{
  return this->started;
}

Path World::smooth_path(cell_path_t path,
                        Position& start, const Position& end, const short width) const
{
  Position current_pos(start);
  Path res;
  // TODO experiment with other step values. Or set step to half the width
  // of the entity?
  Position temp_goal;
  // TODO check if doing that without any A* would not be a lot faster.
  // log_error("Number of cells: " << path.size());
  Position next_pos = this->get_next_path_position(path, start, end, width);
  res.push_back(next_pos);
  // log_error(" ======== Next path position is " << next_pos);
  // log_error("Remaining cells: " << path.size());
  while (next_pos != end)
    {
      next_pos = this->get_next_path_position(path, next_pos, end, width);
      // log_error("NEXT path position is " << next_pos);
      res.push_back(next_pos);

      // next_pos = this->get_next_path_position(path, next_pos, end, width);
      // log_error("NEXT path position is " << next_pos);
      // res.push_back(next_pos);

      // next_pos = this->get_next_path_position(path, next_pos, end, width);
      // log_error("NEXT path position is " << next_pos);
      // res.push_back(next_pos);
    }
  // log_error("Remaining cells: " << path.size());
  return res;
}

Position World::get_next_path_position(cell_path_t& path, const Position current,
                                       const Position& end, const short width) const
{
  const mpreal step = 2;

  bool first_cell = true;
  // First check if the whole path is just a simple straight line.
  if (this->can_walk_in_straight_line(current, end, step, width) == true)
    return end;
  Position temp_goal;
  Position prev_pos(current);
  while (1)
    {
      // assert(path.size() > 0);
      if (path.size() == 0)
        return end;
      std::list<std::size_t>::reverse_iterator rit = path.rend();
      // log_error("length of path in cells: " << path.size());
      rit++;
      std::size_t cell = *(rit);
      if (path.size() > 1)
        {
          rit++;
          std::size_t next_cell = *(rit);
          temp_goal = this->get_nearest_corner(prev_pos,
                                               cell, next_cell, width);
        }
      else
        temp_goal = this->get_nearest_corner(prev_pos, cell, width);
      // log_error("Nearest corner of current position (" << prev_pos << ") is " << temp_goal);
      if (first_cell == false && this->can_walk_in_straight_line(current, temp_goal, step, width) == false)
        {
          // log_error("Nope.");
          return prev_pos;
        }
      first_cell = false;
      // log_error("Yes.");
      prev_pos = temp_goal;
      path.pop_back();
    }
  // return temp_goal;
  // log_error("Length of straight line = " << length_of_straight_line << " && temp_goal = " << temp_goal);
  // return temp_goal;
  // current_path_cell += length_of_straight_line + 1;
}

bool World::can_walk_in_straight_line(const Position& start, const Position& end, const mpreal step, const short width) const
{
  Vec2 forward(end - start);
  // log_error(forward);
  forward.set_length(width/2);
  Position start1 = start // + forward.perpendicular1()
    ;
  Position end1 = end // + forward.perpendicular1()
    ;
  Position start2 = start // + forward.perpendicular2()
    ;
  Position end2 = end // + forward.perpendicular2()
    ;
  // log_error("can_walk_in_straight_line : start = " << start << ", end=" << end << " width=" << width);
  // log_error("From " << start1 << " to " << end1 << ". And from " << start2 << " to " << end2);
  if (this->has_a_line_of_sight(start1, end1, step, width) == false)
    return false;
  if (this->has_a_line_of_sight(start2, end2, step, width) == false)
    return false;
  return true;
}

bool World::has_a_line_of_sight(const Position& start, const Position& end,
                                const mpreal step, const short width) const
{
  // log_error("Distance " << Position::distance(start, end));
  // The point to move along the line to check if everything is walkable.
  Position pointer(start);
  // A vector with a length of 'step' in the direction of the end position
  // We use this vector to move the pointor along the line.
  Vec2 forward(end - start);
  forward.set_length(step);
  // log_error("forward " << forward << " length: " << forward.length());
  // the cell position the pointer is at.
  short prevx, prevy;
  short x, y;
  this->get_cell_at_position(pointer, prevx, prevy);
  // Move the pointer until we reach the destination, or an obstacle.
  log_error("Distance between start and end of path: "<< Position::distance(pointer, end));
  while (Position::distance(pointer, end) >= step)
    {
      // Move the pointer forward.
      pointer += forward;
      // log_error("Pointer: " << pointer);
      this->get_cell_at_position(pointer, x, y);
      // If the cell changed, we check if we can walk from the previous to
      // the current one. If not, it's not walkable and we return false
      if ((prevx != x) || (prevy != y))
        {
          // log_error("Cell changed");
          if (this->can_traverse_cell(prevx, prevy, x, y) == false)
            // we hit an obstabcle.
            {
              // log_error("cannot traverse cell");
              return false;
            }
          // log_error("can traverse cell");
        }
      prevy = y;
      prevx = x;
    }
  this->get_cell_at_position(end, x, y);
  if (((prevx != x) || (prevy != y)) && this->can_traverse_cell(prevx, prevy, x, y) == false)
    {
      // log_error("cannot traverse cell 2");
      return false;
    }
  return true;
}

bool World::can_traverse_cell(const short x, const short y,
                              const short x2, const short y2) const
{
  // TODO in the case of a diagonal move, we currently return true.  Do a
  // more correct thing checking if (for example if we go right-down) we can
  // go right THEN down OR down THEN right. If one of those is possible,
  // return true, otherwise return false.
  // log_error("can_traverse_cell: " << x << ":" << y << " " << x2 << ":" << y2);
  ushort heights = this->map->get_cell_heights(x, y);
  ushort neighbour_heights = this->map->get_cell_heights(x2, y2);
  if (y == y2 && x == x2)
    {
      // log_error("same");
      return true;
    }
  if (y2 == y - 1) // move up
    {
      // log_error("up");
      if (x != x2)
        return false;
      if ((((heights) & 15) == ((neighbour_heights >> 12) & 15)) &&
          (((heights >> 4) & 15) == ((neighbour_heights >> 8) & 15)))
        return true;
      else
        return false;
    }
  else if (y2 == y + 1) // move down
    {
      // log_error("down");
      if (x != x2)
        return false;
      if ((((heights >> 8) & 15) == ((neighbour_heights >> 4) & 15)) &&
          (((heights >> 12) & 15) == ((neighbour_heights) & 15)))
        return true;
      else
        return false;
    }
  else if (x2 == x - 1) // move left
    {
      // log_error("left");
      if (y != y2)
        return false;
      if ((((heights) & 15) == ((neighbour_heights >> 4) & 15)) &&
          (((heights >> 12) & 15) == ((neighbour_heights >> 8) & 15)))
        return true;
      else
        return false;
    }
  else if (x2 == x + 1) // move right
    {
      // log_error("right");
      if (y != y2)
        return false;
      if ((((heights >> 4) & 15) == ((neighbour_heights) & 15)) &&
          (((heights >> 8) & 15) == ((neighbour_heights >> 12) & 15)))
        return true;
      else
        return false;
    }
  assert(false);
  return false;
}

Position World::get_nearest_corner(const Position& pos, const std::size_t cell, const std::size_t next_cell, const short width) const
{
  const uint x = cell % this->map->get_width_in_tiles();
  const uint y = cell / this->map->get_width_in_tiles();
  const uint nx = next_cell % this->map->get_width_in_tiles();
  const uint ny = next_cell / this->map->get_width_in_tiles();
  // log_error("get_nearest_corner " << x << ":" << y << " && " << nx << ":" << ny);
  if (nx == x - 1)
    {
      assert(y == ny);
      Position nearest_position(x * 100 + width, y * 100 + width);
      mpreal shorter_distance = Position::distance(pos, nearest_position);
      Position other_position(x * 100 + width, (y + 1) * 100 - width);
      mpreal other_distance = Position::distance(pos, other_position);
      if (other_distance < shorter_distance)
        return other_position;
      return nearest_position;
    }
  else if (nx == x + 1)
    {
      assert(y == ny);
      Position nearest_position((x + 1) * 100 - width, y * 100 + width);
      mpreal shorter_distance = Position::distance(pos, nearest_position);
      Position other_position((x + 1) * 100 - width, (y + 1) * 100 - width);
      mpreal other_distance = Position::distance(pos, other_position);
      if (other_distance < shorter_distance)
        return other_position;
      return nearest_position;
    }
  else if (ny == y + 1)
    {
      assert(x == nx);
      Position nearest_position(x * 100 + width, (y + 1) * 100 - width);
      mpreal shorter_distance = Position::distance(pos, nearest_position);
      Position other_position((x + 1) * 100 - width, (y + 1) * 100 + width);
      mpreal other_distance = Position::distance(pos, other_position);
      if (other_distance < shorter_distance)
        return other_position;
      return nearest_position;
    }
  else if (ny == y - 1)
    {
      assert(x == nx);
      Position nearest_position(x * 100 + width, y * 100 + width);
      mpreal shorter_distance = Position::distance(pos, nearest_position);
      Position other_position((x + 1) * 100 - width, y * 100 + width);
      mpreal other_distance = Position::distance(pos, other_position);
      if (other_distance < shorter_distance)
        return other_position;
      return nearest_position;
    }
  assert(false);
}

Position World::get_nearest_corner(const Position& pos, const std::size_t cell, const short width) const
{
  const uint x = cell % this->map->get_width_in_tiles();
  const uint y = cell / this->map->get_width_in_tiles();
  Position nearest_position(x * 100 + width, y * 100 + width);
  mpreal shorter_distance = Position::distance(pos, nearest_position);
  Position other_position(x * 100 + width, (y + 1) * 100 - width);
  mpreal other_distance = Position::distance(pos, other_position);
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
