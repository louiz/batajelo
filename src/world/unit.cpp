#include <world/unit.hpp>
#include <world/world.hpp>
#include <world/work.hpp>

Unit::Unit():
  Entity(),
  width(20),
  speed(2)
{
}

Unit::~Unit()
{
}

Unit::Unit(const Unit& model):
  Entity(model),
  pos(model.pos),
  width(model.width),
  height(model.height),
  health(model.health),
  speed(model.speed)
{
  log_debug("Creating new unit(" << this->type_id << ") of id: " << this->id);
}

bool Unit::contains(const Position& pos) const
{
  if ((pos.x >= this->pos.x) && (pos.y >= this->pos.y) &&
      (pos.x <= this->pos.x + this->width) && (pos.y <= this->pos.y + this->height))
    return true;
  return false;
}

bool Unit::build(World* world, Work* w)
{
  BuildWork* work = dynamic_cast<BuildWork*>(w);
  assert(work);
  if (work->building == 0)
    {
      log_info("creating building");
      work->building = world->create_building(work->id, work->x, work->y);
      world->insert_building(work->building);
    }
  return false;
}

bool Unit::follow_path(World* world, Work* w)
{
  PathWork* work = dynamic_cast<PathWork*>(w);
  assert(work);
  if (work->path.size() == 0)
    {
      if (work->calculated == false)
        {
          log_warning("Calculating path: " << this->pos);
          work->path = world->calculate_path(work->end_position, this);
          work->calculated = true;
        }
      else
        return true;
    }
  Position goal = work->path.front();
  Vec2 movement(goal - this->pos);
  if (movement.length() == 0)
    // Movement is too short to have something != 0 because of the Fix16 precision.
    // We consider to be at the goal.
    {
      work->path.pop_front();
      return false;
    }
  // log_error(movement.length() / speed);
  movement.set_length(this->speed);
  // Adjust movement here, to avoid obstacles, etc etc.

  if (Position::distance(goal, this->pos) < this->speed)
    movement.set_length(Position::distance(goal, this->pos));
  else
    movement.set_length(this->speed);
  // log_error("Movement: " << movement);
  Unit* unit_ahead;
  if ((unit_ahead = this->get_unit_ahead(movement, world)) != 0)
    {
      if ((work->path.size() == 1) && (unit_ahead->is_obstructing_position(this, goal) == true))
        {
          // We considere the goal to be reached
          work->path.pop_front();
          return false;
        }
    }
  if (is_wall_ahead(movement, world) == true)
    {
      if ((work->path.size() == 1) && (Position::distance(this->pos, goal) <= this->width/2))
        {
          work->path.pop_front();
          return false;
        }
    }
  this->steer_to_avoid_obstacle(movement, world);
  this->pos += movement;
  return false;
}

bool Unit::is_obstructing_position(Entity* entity, const Position& position) const
{
  // if (this->path.size() == 0)
  //   return false;
  // if (Position::distance(this->pos, position) > (entity->width + this->width))
  return false;
  // return true;
}

void Unit::steer_to_avoid_obstacle(Vec2& mov, World* world) const
{
  int i = 0;
  bool turn_direction;
  while (this->will_collide_if_we_move(mov, world) == true)
    {
      if (turn_direction == true)
        {
          // log_error("Rotate " << 0.1*i);
          mov.rotate(0.5 * i);
        }
      else
        {
          // log_error("Rotate " << -(0.1*i));
          mov.rotate(-(0.5 * i));
        }
      turn_direction = !turn_direction;
      if (i++ == 13)
        {
          mov = Vec2::zero;
          return ;
        }
    }
}

Unit* Unit::get_unit_ahead(const Vec2& mov, World* world) const
{
  Unit* unit;
  std::list<Unit*>::iterator it;
  for (it = world->units.begin(); it != world->units.end(); ++it)
    {
      unit = *it;
      if (unit == this)
        continue;
      if (Position::distance(this->pos + mov, unit->pos) <= (unit->width + this->width))
        {
          return unit;
        }
    }
  return 0;
}

bool Unit::will_collide_if_we_move(Vec2& mov, World* world) const
{
  Unit* unit = this->get_unit_ahead(mov, world);
  if (unit != 0)
    return true;
  if (this->is_wall_ahead(mov, world) == true)
    return true;
  return false;
}

bool Unit::is_wall_ahead(Vec2& mov, World* world) const
{
  Position pos_after(this->pos + mov);
  // log_error("Is_Wall_Ahead: " << this->pos << " and " << pos_after);
  // mov_with_width.set_length(mov_with_width.length() + this->width/2);
  short x;
  short y;
  world->get_cell_at_position(this->pos, x, y);
  short xa;
  short ya;
  world->get_cell_at_position(pos_after, xa, ya);
  if ((x != xa) || (y != ya))
    {
      if (world->can_traverse_cell(x, y, xa, ya) == false)
        {
          return true ;
        }
      // else
      //   log_error("WE CAN TRAVERSE CELL");
    }
  return false;
}

bool Unit::can_be_pushed() const
{
  // if (this->path.size() == 0)
  //   return true;
  // else
  return false;
}
