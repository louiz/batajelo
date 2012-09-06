#ifndef __ENTITY_HPP__
# define __ENTITY_HPP__

#include <logging/logging.hpp>
#include <world/path.hpp>
#include <world/position.hpp>
#include <serialization/serializable.hpp>

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
  // Entity(const Entity& model, const SerializableEntity& e);
  virtual ~Entity();
  bool contains(const Position&) const;
  void update_health();
  unsigned short get_id() const { return this->id; }

  static unsigned short current_id;
  static unsigned short current_type_id;

  virtual void tick(World*);
  void follow_path(World*);
  void set_path(const Path& path);
  void cancel_path();
  void steer_to_avoid_obstacle(Vec2&, World*) const;
  /**
   * Returns a pointer to the entity just ahead of us (that means that if we
   * follow the givement vector movement, we will collide with it). If
   * there's none, NULL is returned.
   */
  Entity* get_entity_ahead(const Vec2&, World*) const;
  /**
   * Returns weither or not this entity (if not moving) makes it impossible
   * for the given entity to reach this Position. This means that this
   * entity is OVER the position, or close enough that the moving entity
   * cannot reach it because of its width.
   */
  bool is_obstructing_position(Entity*, const Position&) const;
  /**
   * Return true if we will collide with a wall OR a unit if we move
   * following that vector.
   */
  bool will_collide_if_we_move(Vec2& mov, World*) const;
  /**
   * Returns weither or not this entity will go into a wall if we move on
   * following the given movement.
   */
  bool is_wall_ahead(Vec2& mov, World* world) const;
  /**
   * Return whether or not the unit can be pushed by an other entity.
   Mostly, if it is not doing any action and is not on Hold position, it
   can.
   */
  bool can_be_pushed() const;

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & pos & type_id;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
   ar & pos & type_id;
  }

private:
  Entity& operator=(const Entity&);
public:
  Position pos;
  /**
   * The width
   */
  short width;
  /**
   * The height
   */
  int16_t height;
  /**
   * An uniq id for the entity.
   */
  unsigned short id;
  /**
   * A uniq id for the entity type.
   */
  unsigned short type_id;
  /**
   * The path to follow. If it is 0, the entity is not moving.
   */
  Path path;
  /**
   * The entity health.
   */
  uint health;
};

#endif // __ENTITY_HPP__
