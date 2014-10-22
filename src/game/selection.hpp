/** @addtogroup World
 *  @{
 */

/**
 * Defines a list of entities, as part of a selection. For example the
 * currently selected entities are inside one Selection, each control group
 * is a distinct Selection, etc.
 */

#ifndef __SELECTION_HPP__
# define __SELECTION_HPP__

#include <world/entity.hpp>

#include <list>
#include <vector>

typedef std::function<void(void)> t_selection_changed_callback;

/**
 * A selection cannot contain more entities than that.
 */
#define MAX_SELECTION_SIZE 240

class Selection
{
public:
  Selection();
  ~Selection();
  bool is_in_selection(const Entity*) const;
  void add_to_selection(const Entity*);
  void remove_from_selection(const Entity*);
  void clear();
  bool is_empty() const;
  const std::list<const Entity*>& get_entities() const;
  std::size_t size() const;
  void on_modified() const;

  std::vector<t_selection_changed_callback> on_modified_callbacks;

private:
  Selection(const Selection&);
  Selection& operator=(const Selection&);

  std::list<const Entity*> entities;
};

#endif // __MOUSE_SELECTION_HPP__

