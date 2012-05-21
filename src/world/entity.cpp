#include <world/entity.hpp>

unsigned short Entity::current_id = 0;
unsigned short Entity::current_type_id = 0;

Entity::Entity():
  x(0),
  y(0),
  width(20),
  height(20),
  selected(false),
  type_id(++Entity::current_type_id),
  path(0),
  health(0)
{
}

Entity::Entity(const Entity& e):
  id(++Entity::current_id),
  path(0)
{
  this->y = e.y;
  this->x = e.x;
  this->width = e.width;
  this->height = e.height;
  this->selected = e.selected;
  this->type_id = e.type_id;
  this->health = e.health;
  if (e.path != 0)
    this->set_path(*(e.path));
  log_debug("Creating new unit of id: " << this->id);
}

Entity::~Entity()
{
}

bool Entity::is_selected() const
{
  return this->selected;
}

bool Entity::contains(unsigned int x, unsigned int y) const
{
  log_debug(x << ":" << y);
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
  // TODO use speed, and move correctly.
  if (this->path == 0)
    return ;
  if ((this->x == this->path->x) && (this->y == this->path->y))
    {
      this->cancel_path();
      return ;
    }
  if (this->x < this->path->x)
    ++this->x;
  else
    --this->x;
  if (this->y < this->path->y)
    ++this->y;
  else
    --this->y;
}

void Entity::update_health()
{
  if (this->x > 50 && this->y > 50 &&
      this->x < 350 && this->y < 350)
    this->health++;
}
