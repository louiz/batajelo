#include <world/entity.hpp>
#include <cmath>
#include <world/serializable_entity.hpp>

unsigned short Entity::current_id = 0;
unsigned short Entity::current_type_id = 0;

Entity::Entity():
  pos(0, 0),
  width(30),
  height(0),
  selected(false),
  type_id(Entity::current_type_id++),
  // path(0),
  health(0)
{
  log_debug("Creating new unit model: id=" << this->type_id);
}

Entity::Entity(const Entity& model):
  id(++Entity::current_id),
  selected(false)
  // path(0)
{
  this->pos = model.pos;
  this->width = model.width;
  this->height = model.height;
  this->type_id = model.type_id;
  this->health = model.health;
  log_debug("Creating new unit(" << this->type_id << ") of id: " << this->id);
}

Entity::Entity(const Entity& model, const SerializableEntity& e):
  id(++Entity::current_id),
  selected(false),
  // path(0),
  health(0)
{
  this->width = model.width;
  this->height = model.height;
  this->selected = model.selected;
  this->type_id = model.type_id;
  this->health = model.health;
  // this->path = 0;
  this->pos = Position(e.x, e.y);
  log_debug("Creating new unit(" << this->type_id << ") of id: " << this->id);
}

Entity::~Entity()
{
}

bool Entity::is_selected() const
{
  return this->selected;
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

void Entity::tick()
{
  this->follow_path();
  this->update_health();
}

void Entity::follow_path()
{
  mpreal speed(5);
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
  if (Position::distance(goal, this->pos) < speed)
    movement.set_length(Position::distance(goal, this->pos));
  else
    movement.set_length(speed);
  this->pos += movement;
}

void Entity::update_health()
{
}
