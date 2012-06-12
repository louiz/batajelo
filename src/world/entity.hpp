#ifndef __ENTITY_HPP__
# define __ENTITY_HPP__

#include <logging/logging.hpp>
#include <serialization/serializable.hpp>
#include <world/path.hpp>

class Camera;
class World;

class Entity: public Serializable
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
  bool is_selected() const;
  bool contains(unsigned int, unsigned int) const;

  unsigned short get_id() const { return this->id; }

  static unsigned short current_id;
  static unsigned short current_type_id;

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & x & y & width & height & selected & id & type_id;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
   ar & x & y & width & height & selected & id & type_id;
  }

  virtual void tick();
  void follow_path();
  void update_health();

  void set_path(const Path& path);
  void cancel_path();

private:
  Entity& operator=(const Entity&);
public:
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

  unsigned health;

};

#endif // __ENTITY_HPP__
