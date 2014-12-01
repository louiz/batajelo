#ifndef __ENTITY_HPP__
# define __ENTITY_HPP__

#include <world/position.hpp>

#include <cstdint>
#include <memory>
#include <queue>
#include <list>

using EntityType = uint16_t;
using EntityId = uint16_t;

class Camera;
class World;
class Work;
class Unit;

class Entity
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
  virtual ~Entity();

  unsigned short get_id() const { return this->id; }
  /**
   * Returns whether or not the given position is under the entity
   */
  virtual bool contains(const Position&) const = 0;
  /**
   * Regularly update the entity.
   */
  void tick(World*);

  void clear_works();
  void set_work(std::unique_ptr<Work>&&);
  void queue_work(std::unique_ptr<Work>&&);
  /**
   * Returns weither or not this entity (if not moving) makes it impossible
   * for the given unit to reach this Position. This means that this
   * entity is OVER the position, or close enough that the moving entity
   * cannot reach it because of its width.
   */
  virtual bool is_obstructing_position(const Unit*, const Position&) const = 0;

private:
  Entity& operator=(const Entity&);
public:
  static EntityId current_id;
  static EntityType current_type_id;
  /**
   * An uniq id for the entity.
   */
  EntityId id;
  /**
   * A uniq id for the entity type.
   */
  EntityType type_id;
  /**
   * The entity name
   */
  std::string name;
  /**
   * A serie of works that the entity has to execute.  For example
   * World::do_path will add a PathWork to the entity, which contains a path
   * and a pointer to Unit::follow_path, receiving that PathWork structure
   * as an argument. World::do_build will push a PathWork (to go to the cell
   * to build) and a BuildWork.
   *
   * Once the Work is done (destination reached, or building complete, or
   * anything), we pop that work. Many works can be queued. For example if
   * we do a shift-move, the PathWork is added at the end of the list. If
   * not, it just replaces all the works in the list.
   *
   * A list is used instead of a queue, though a queue would be almost
   * perfect (push back, pop front only), but we need to be able to traverse
   * the list to check their value etc. So we just use a list and push_back
   * and pop_front instead. Should be almost as effecient.
   */
  std::list<std::unique_ptr<Work>> works;
};

#endif // __ENTITY_HPP__
