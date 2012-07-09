#ifndef __ENTITY_HPP__
# define __ENTITY_HPP__

#include <logging/logging.hpp>
#include <world/path.hpp>
#include <mpreal/mpreal.h>

class Camera;
class World;
class SerializableEntity;

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
  Entity(const Entity& model, const SerializableEntity& e);
  virtual ~Entity();
  bool is_selected() const;
  bool contains(const mpreal&, const mpreal&) const;
  void update_health();
  unsigned short get_id() const { return this->id; }

  static unsigned short current_id;
  static unsigned short current_type_id;

  virtual void tick();
  void follow_path();
  void set_path(const Path& path);
  void cancel_path();

private:
  Entity& operator=(const Entity&);
public:
  /**
   * The left position
   */
  mpreal x;
  /**
   * The top position
   */
  mpreal y;
  /**
   * The width
   */
  int16_t width;
  /**
   * The height
   */
  int16_t height;
  /**
   * Whether or not the entity is selected.
   */
  bool selected;
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
  Path* path;
  /**
   * The entity health.
   */
  uint health;
};

#endif // __ENTITY_HPP__
