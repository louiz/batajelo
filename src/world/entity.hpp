#ifndef __ENTITY_HPP__
# define __ENTITY_HPP__

#include <logging/logging.hpp>

class Camera;
class World;

class Entity
{
  friend class Camera;
  friend class World;

public:
  Entity();
  ~Entity();
  bool is_selected() const;
  bool contains(unsigned int, unsigned int) const;

  unsigned short get_id() const { return this->id; }

  static unsigned short current_id;

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
  /**
   * The width
   */
  unsigned int width;
  /**
   * The height
   */
  unsigned int height;
  /**
   * Whether or not the entity is selected.
   */
  bool selected;

  unsigned short id;
};

#endif // __ENTITY_HPP__
