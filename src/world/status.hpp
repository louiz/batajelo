#ifndef STATUS_HPP_INCLUDED
#define STATUS_HPP_INCLUDED

class Entity;
class World;

/**
 * A Status affects one entity. It can do things to the entity itself or the
 * surrounding world, each tick, when applied and or when it expires.
 *
 * A status can be set on an entity for example by casting an ability on it,
 * when a projectile hits it, when it enters some area or some range, etc.
 */

class Status
{
public:
  Status(Entity* entity, World* world):
    entity(entity),
    world(world)
  {
  }
  virtual ~Status()
  {
    this->unapply();
  }
  /**
   * Called when the status is being set on the entity
   */
  virtual void apply() {}
  /**
   * Called when the status is being removed from the entity
   */
  virtual void unapply() {}
  /**
   * Called on each world tick
   */
  virtual void tick() {}

protected:
 /**
  * Non-owning pointer, just to have access to the affected entity and the
  * world on tick(), apply(), unapply() etc
  */
  Entity* entity;
  World* world;

private:
  Status(const Status&) = delete;
  Status(Status&&) = delete;
  Status& operator=(const Status&) = delete;
  Status& operator=(Status&&) = delete;
};


#endif /* STATUS_HPP_INCLUDED */
