#include <logging/logging.hpp>
#include <world/building.hpp>
#include <world/unit.hpp>
#include <world/world.hpp>

Building::Building()
{
}

Building::~Building()
{
}

Building::Building(const Building& o):
  Entity(o),
  x(o.x),
  y(o.y),
  height(o.height),
  cells(o.cells)
{
}

bool Building::is_obstructing_position(const Unit* unit, const Position& position) const
{
  return false;
}

bool Building::contains(const Position&) const
{
  return false;
}

bool Building::spawn(World* world, Work* w)
{
  SpawnWork* work = dynamic_cast<SpawnWork*>(w);
  assert(work);
  const Unit* model = world->get_unit_model(work->type_id);
  if (work->elapsed_time != model->spawn_duration)
    {
      work->elapsed_time++;
      return false;
    }
  // maybe just use the model here, and not a new created unit that we will
  // delete if we cannot spawn it?
  Unit* unit = world->create_unit(work->type_id);
  // find free spawning position

  bool position_found = false;
  const unsigned short diff = unit->width * 2 + 1;
  Position pos(this->x * CELL_SIZE + ((this->cells * CELL_SIZE) / 2),
               this->y * CELL_SIZE + (this->cells * CELL_SIZE));
  const Vec2 top(0, -diff);
  const Vec2 right(diff, 0);
  const Vec2 bot(0, diff);
  const Vec2 left(-diff, 0);

  std::vector<Vec2> directions = {right, bot, left, left, top, top, right, right, right, bot, bot, bot, left, left, left, left, top, top, top, top, right, right, right, right, right, bot, bot, bot, bot, bot, left, left, left, left, left, left};  // Yeah, I know, right?
  unit->pos = Vec2::zero;

  for (std::vector<Vec2>::const_iterator it = directions.begin(); it != directions.end(); ++it)
    {
      if (world->can_unit_spawn_at_pos(unit, pos) == true)
        {
          unit->pos = pos;
          break ;
        }
      pos += *it;
    }
  if (unit->pos == Vec2::zero)
    {
      delete unit;
      return false;
    }
  if (this->rally_point != Vec2::zero)
    {
      PathWork* path_work = new PathWork(unit, this->rally_point);
      unit->set_work(path_work);
    }
  world->insert_unit(unit);
  return true;
}

bool Building::set_rally_point(World* world, Work* w)
{
  RallyWork* work = dynamic_cast<RallyWork*>(w);
  assert(work);
  this->rally_point = work->position;
  log_info("Setting rally point to: " << this->rally_point);
  return true;
}

Position Building::get_center_position() const
{
  Position pos;
  pos.x = this->x * CELL_SIZE + ((this->cells * CELL_SIZE) / 2);
  pos.y = this->y * CELL_SIZE + ((this->cells * CELL_SIZE) / 2);
  return pos;
}

std::vector<unsigned short> Building::get_spawning_queue() const
{
  std::vector<unsigned short> res;
  const SpawnWork* work;
  for (std::list<Work*>::const_iterator it = this->works.begin();
       it != this->works.end();
       ++it)
    {
      work = dynamic_cast<const SpawnWork*>(*it);
      if (!work)
        continue ;
      res.push_back(work->type_id);
    }
  return res;
}

unsigned int Building::get_spawn_time_elapsed() const
{
  const SpawnWork* work = dynamic_cast<const SpawnWork*>(this->works.front());
  assert(work); // we should not call this method if we are not currently
                // spawning something
  return work->elapsed_time;
}
