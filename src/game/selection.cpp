#include <logging/logging.hpp>
#include <game/selection.hpp>
#include <assert.h>

#include <algorithm>

Selection::Selection()
{
}

Selection::~Selection()
{
}

bool Selection::is_in_selection(const Entity* entity) const
{
  return std::find_if(this->entities.begin(),
                      this->entities.end(),
                      [entity](const Entity* other)
                      {
                        return other == entity;
                      }) != this->entities.end();
}

void Selection::add_to_selection(const Entity* entity)
{
  assert(this->is_in_selection(entity) == false);
  // TODO maybe keep the list sorted by some criterion
  if (this->entities.size() == MAX_SELECTION_SIZE)
    return ;
  this->entities.push_back(entity);
  this->on_modified();
}

void Selection::remove_from_selection(const Entity* entity)
{
  auto it = std::find(this->entities.begin(),
                      this->entities.end(),
                      entity);
  if (it != this->entities.end())
    {
      this->entities.erase(it);
      this->on_modified();
    }
}

void Selection::clear()
{
  this->entities.clear();
  this->on_modified();
}

bool Selection::is_empty() const
{
  return this->entities.empty();
}

const std::vector<const Entity*>& Selection::get_entities() const
{
  return this->entities;
}

std::size_t Selection::size() const
{
  return this->entities.size();
}

void Selection::on_modified() const
{
  for (std::vector<t_selection_changed_callback>::const_iterator it = this->on_modified_callbacks.begin();
       it != this->on_modified_callbacks.end(); ++it)
    {
      (*it)();
    }
}
