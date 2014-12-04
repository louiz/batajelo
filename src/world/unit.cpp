 #include <logging/logging.hpp>
#include <world/unit.hpp>
#include <world/world.hpp>
#include <world/work.hpp>

Unit::Unit():
  Entity(),
  width(20),
  speed(8)
{
}

Unit::~Unit()
{
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
  return false;
}

void Unit::follow_path(Path& path, World* world)
{
  Position goal = path.front();
  Vec2 movement(goal - this->pos);
  if (movement.length() == 0)
    {
      // Movement is too short to have something != 0 because of the Fix16 precision.
      // We consider to be at the goal.
      path.pop_front();
      return ;
    }

  if (Position::distance(goal, this->pos) < this->speed)
    movement.set_length(Position::distance(goal, this->pos));
  else
    movement.set_length(this->speed);
  this->pos += movement;
}

bool Unit::is_obstructing_position(const Unit* unit, const Position& position) const
{
  if (Position::distance(this->pos, position) > (unit->width + this->width))
    return false;
  return true;
}

void Unit::steer_to_avoid_obstacle(Vec2& mov, World* world) const
{
  int i = 0;
  bool turn_direction;
  while (this->will_collide_if_we_move(mov, world) == true)
    {
      if (turn_direction == true)
        mov.rotate(0.5 * i);
      else
        mov.rotate(-(0.5 * i));
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
  return nullptr;
}

bool Unit::will_collide_if_we_move(Vec2& mov, World* world) const
{
  Unit* unit = this->get_unit_ahead(mov, world);
  if (unit != nullptr)
    return true;
  if (this->is_wall_ahead(mov, world) == true)
    return true;
  return false;
}

bool Unit::is_wall_ahead(Vec2& mov, World* world) const
{
  Position pos_after(this->pos + mov);
  short x;
  short y;
  std::tie(x, y) = world->get_cell_at_position(this->pos);
  short xa;
  short ya;
  std::tie(xa, ya) = world->get_cell_at_position(pos_after);
  if ((x != xa) || (y != ya))
    {
      if (world->can_traverse_cell(x, y, xa, ya) == false)
        {
          return true ;
        }
    }
  return false;
}

bool Unit::can_be_pushed() const
{
  return false;
}
