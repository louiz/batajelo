#ifndef __ENTITY_HPP__
# define __ENTITY_HPP__

#include <logging/logging.hpp>

class Camera;

class Entity
{
  friend class Camera;
public:
  Entity();
  ~Entity();
  bool is_selected() const;
  bool contains(unsigned int, unsigned int) const;

private:
  Entity(const Entity&);
  Entity& operator=(const Entity&);
  /**
   * The left position
   */
  unsigned int x;
  /**
   * The top position
   */
  unsigned int y;
  unsigned int width;
  unsigned int height;
  /**
   * Whether or not the entity is selected.
   */
  bool selected;
};

#endif // __ENTITY_HPP__
