#include <world/selection.hpp>

Selection::Selection()
{
}

Selection::~Selection()
{
}

bool Selection::is_in_selection(const Entity* entity) const
{
  std::list<const Entity*>::const_iterator it;
  for (it = this->entities.begin(); it != this->entities.end(); ++it)
    {
      if ((*it) == entity)
        return true;
    }
  return false;
}

void Selection::add_to_selection(const Entity* entity)
{
  assert(this->is_in_selection(entity) == false);
  // TODO maybe keep the list sorted by some criterion
  if (this->entities.size() == MAX_SELECTION_SIZE)
    return ;
  this->entities.push_back(entity);
}

void Selection::remove_from_selection(const Entity* entity)
{
    std::list<const Entity*>::iterator it;
  for (it = this->entities.begin(); it != this->entities.end(); ++it)
    {
      if ((*it) == entity)
        {
          this->entities.erase(it);
          return ;
        }
    }
  log_error("Tried to remove an entity from a Selection that does'nt contain it.");
  assert(false);
}

void Selection::clear()
{
  this->entities.clear();
}

bool Selection::is_empty() const
{
  return this->entities.empty();
}

const std::list<const Entity*>& Selection::get_entities() const
{
  return this->entities;
}

std::size_t Selection::size() const
{
  return this->entities.size();
}
