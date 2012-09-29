#ifndef __ENTITY_HPP__
# define __ENTITY_HPP__

#include <logging/logging.hpp>
#include <serialization/serializable.hpp>
#include <world/position.hpp>

class Camera;
class World;

class Entity: Serializable
{
  friend class Camera;
  friend class World;

public:
  /**
   * This constructor is used when creating a model.
   */
  Entity();
  /**
   * This constructor is used when creating a unit, based
   * on a model.
   */
  Entity(const Entity&);
virtual ~Entity() {};

  unsigned short get_id() const { return this->id; }
  /**
   * Returns whether or not the given position is under the entity
   */
  virtual bool contains(const Position&) const = 0;
  /**
   * Regularly update the entity.
   */
  virtual void tick(World*) = 0;

  /**
   * Returns weither or not this entity (if not moving) makes it impossible
   * for the given entity to reach this Position. This means that this
   * entity is OVER the position, or close enough that the moving entity
   * cannot reach it because of its width.
   */
  virtual bool is_obstructing_position(Entity*, const Position&) const = 0;

  virtual void serialize(oarchive & ar, const unsigned int) = 0;
  virtual void serialize(iarchive & ar, const unsigned int) = 0;

private:
  Entity& operator=(const Entity&);
public:
  static unsigned short current_id;
  static unsigned short current_type_id;
  /**
   * An uniq id for the entity.
   */
  unsigned short id;
  /**
   * A uniq id for the entity type.
   */
  unsigned short type_id;
  /**
   * The entity name
   */
  std::string name;
};

#endif // __ENTITY_HPP__
