#include <world/entity.hpp>
#include <cmath>
#include <world/world.hpp>

unsigned short Entity::current_id = 0;
unsigned short Entity::current_type_id = 0;

Entity::Entity():
  pos(0, 0),
  width(15),
  height(0),
  type_id(Entity::current_type_id++),
  // path(0),
  health(0)
{
  log_debug("Creating new unit model: id=" << this->type_id);
}

Entity::Entity(const Entity& model):
  id(++Entity::current_id)
  // path(0)
{
  this->pos = model.pos;
  this->width = model.width;
  this->height = model.height;
  this->type_id = model.type_id;
  this->health = model.health;
  log_debug("Creating new unit(" << this->type_id << ") of id: " << this->id);
}

// Entity::Entity(const Entity& model, const SerializableEntity& e):
//   selected(false),
//   id(++Entity::current_id),
//   // path(0),
//   health(0)
// {
//   this->width = model.width;
//   this->height = model.height;
//   this->selected = model.selected;
//   this->type_id = model.type_id;
//   this->health = model.health;
//   // this->path = 0;
//   this->pos = Position(e.x, e.y);
//   log_debug("Creating new unit(" << this->type_id << ") of id: " << this->id);
// }

Entity::~Entity()
{
}

bool Entity::contains(const Position& pos) const
{
  if ((pos.x >= this->pos.x) && (pos.y >= this->pos.y) &&
      (pos.x <= this->pos.x + this->width) && (pos.y <= this->pos.y + this->height))
    return true;
  return false;
}

// void Entity::set_path(const Path& path)
// {
//   // if (this->path != 0)
//   //   delete this->path;
//   // this->path = new Path(path);
// }

// void Entity::cancel_path()
// {
//   if (this->path != 0)
//     delete this->path;
//   this->path = 0;
// }

void Entity::tick(World* world)
{
  // log_error("Entity.tick(): " << this->id);
  this->follow_path(world);
  this->update_health();
}

void Entity::follow_path(World* world)
{
  mpreal speed(1.8f);
  // return ;
  if (this->path.size() == 0)
    return ;
  if (this->pos == this->path.front())
    this->path.pop_front();
  if (this->path.size() == 0)
    return ;
  Position goal = this->path.front();
  // log_error("Current: " << this->pos << ". Goal: " << goal);
  Vec2 movement(goal - this->pos);
  // log_error("Movement: " << movement << " length: " << movement.length());
  if (movement.length() == 0)
    // Movement is too short to have something != 0 because of the Fix16 precision.
    // We consider to be at the goal.
    {
      this->path.pop_front();
      return ;
    }
  // log_error(movement.length() / speed);
  movement.set_length(speed);
  // Adjust movement here, to avoid obstacles, etc etc.

  if (Position::distance(goal, this->pos) < speed)
    movement.set_length(Position::distance(goal, this->pos));
  else
    movement.set_length(speed);
  // log_error("Movement: " << movement);
  Entity* entity_ahead;
  if ((entity_ahead = this->get_entity_ahead(movement, world)) != 0)
    {
      if ((this->path.size() == 1) && (entity_ahead->is_obstructing_position(this, goal) == true))
        {
          // We considere the goal to be reached
          this->path.pop_front();
          return ;
        }
    }
  if (is_wall_ahead(movement, world) == true)
    {
      if ((this->path.size() == 1) && (Position::distance(this->pos, goal) <= this->width/2))
        {
          this->path.pop_front();
          return ;
        }
    }
  this->steer_to_avoid_obstacle(movement, world);
  this->pos += movement;
}

bool Entity::is_obstructing_position(Entity* entity, const Position& position) const
{
  // if (this->path.size() == 0)
  //   return false;
  if (Position::distance(this->pos, position) > (entity->width + this->width))
    return false;
  return true;
}

void Entity::steer_to_avoid_obstacle(Vec2& mov, World* world) const
{
  int i = 0;
  bool turn_direction;
  while (this->will_collide_if_we_move(mov, world) == true)
    {
      // if (turn_direction == true)
      //   {
      //     // log_error("Rotate " << 0.1*i);
      //     mov.rotate(0.5 * i);
      //   }
      // else
      //   {
      //     // log_error("Rotate " << -(0.1*i));
      //     mov.rotate(-(0.5 * i));
      //   }
      // turn_direction = !turn_direction;
      // if (i++ == 13)
      //   {
          mov = Vec2::zero;
          return ;
        // }
    }
}

Entity* Entity::get_entity_ahead(const Vec2& mov, World* world) const
{
  Entity* entity;
  std::list<Entity*>::iterator it;
  for (it = world->entities.begin(); it != world->entities.end(); ++it)
    {
      entity = *it;
      if (entity == this)
        continue;
      if (Position::distance(this->pos + mov, entity->pos) <= (entity->width + this->width))
        {
          return entity;
        }
    }
  return 0;
}

bool Entity::will_collide_if_we_move(Vec2& mov, World* world) const
{
  Entity* entity = this->get_entity_ahead(mov, world);
  if (entity != 0)
    return true;
  if (this->is_wall_ahead(mov, world) == true)
    return true;
  return false;
}

bool Entity::is_wall_ahead(Vec2& mov, World* world) const
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

void Entity::update_health()
{
}

bool Entity::can_be_pushed() const
{
  if (this->path.size() == 0)
    return true;
  else
    return false;
}
