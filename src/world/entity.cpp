#include <world/entity.hpp>
#include <cmath>
#include <world/serializable_entity.hpp>

unsigned short Entity::current_id = 0;
unsigned short Entity::current_type_id = 0;

Entity::Entity():
  x(0),
  y(0),
  width(30),
  height(0),
  selected(false),
  type_id(Entity::current_type_id++),
  path(0),
  health(0)
{
  log_debug("Creating new unit model: id=" << this->type_id);
}

Entity::Entity(const Entity& model):
  id(++Entity::current_id),
  selected(false),
  path(0)
{
  this->y = model.y;
  this->x = model.x;
  this->width = model.width;
  this->height = model.height;
  this->type_id = model.type_id;
  this->health = model.health;
  log_debug("Creating new unit(" << this->type_id << ") of id: " << this->id);
}

Entity::Entity(const Entity& model, const SerializableEntity& e):
  id(++Entity::current_id),
  selected(false),
  path(0),
  health(0)
{
  this->width = model.width;
  this->height = model.height;
  this->selected = model.selected;
  this->type_id = model.type_id;
  this->health = model.health;
  this->path = 0;
  this->x = e.x;
  this->y = e.y;
  log_debug("Creating new unit(" << this->type_id << ") of id: " << this->id);
}

Entity::~Entity()
{
}

bool Entity::is_selected() const
{
  return this->selected;
}

bool Entity::contains(const mpreal& x, const mpreal& y) const
{
  if ((x >= this->x) && (y >= this->y) &&
      (x <= this->x + this->width) && (y <= this->y + this->height))
    return true;
  return false;
}

void Entity::set_path(const Path& path)
{
  if (this->path != 0)
    delete this->path;
  this->path = new Path(path);
}

void Entity::cancel_path()
{
  if (this->path != 0)
    delete this->path;
  this->path = 0;
}

void Entity::tick()
{
  this->follow_path();
  this->update_health();
}

void Entity::follow_path()
{
  if (this->path == 0)
    return ;
  mpreal speed(3.0);
  mpreal xdist;
  if (this->x > this->path->x)
    xdist = this->x - this->path->x;
  else
    xdist = this->path->x - this->x;
  mpreal ydist;
  if (this->y > this->path->y)
    ydist = this->y - this->path->y;
  else
    ydist = this->path->y - this->y;
  mpreal dist = xdist + ydist;
  if (speed >= dist)
    {
      this->x = this->path->x;
      this->y = this->path->y;
      this->cancel_path();
      return ;
    }
  mpreal xspeed;
  mpreal yspeed;
  xspeed = xdist/dist*speed;
  yspeed = ydist/dist*speed;
  if (this->x < this->path->x)
    this->x += xspeed;
  else
    this->x -= xspeed;
  if (this->y < this->path->y)
    this->y += yspeed;
  else
    this->y -= yspeed;
}

void Entity::update_health()
{
}
