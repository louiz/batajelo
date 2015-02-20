#ifndef __ENTITY_HPP__
# define __ENTITY_HPP__

#include <world/components.hpp>
#include <world/status.hpp>

#include <cstdint>
#include <memory>
#include <list>

using EntityId = uint16_t;
using EntityType = uint16_t;

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
  Entity(const EntityType& type);
  ~Entity();

  EntityId get_id() const { return this->id; }
  EntityType get_type() const { return this->type; }

  /**
   * Regularly update the entity.
   */
  void tick(World*);

  void clear_works();
  void set_work(std::unique_ptr<Work>);
  void queue_work(std::unique_ptr<Work>);

  template <typename ComponentClass>
  ComponentClass* get() const
  {
    static_assert(std::is_base_of<Component, ComponentClass>::value,
                  "ComponentClass must be a Component.");
    static_assert(ComponentClass::component_type != ComponentType::Invalid,
                  "ComponentClass must set its own type.");
    auto index = static_cast<std::size_t>(ComponentClass::component_type);
    return static_cast<ComponentClass*>(this->components[index].get());
  }

  template <typename ComponentClass>
  void add_component(ComponentClass&& pointer)
  {
    auto index = static_cast<std::size_t>(ComponentClass::element_type::component_type);
    this->components[index] = std::move(pointer);
  }
  template <typename StatusType, typename... ArgsType>
  void add_status(World* world, ArgsType&&... args)
  {
    this->status.push_back(std::make_unique<StatusType>(this, world,
                                                        std::forward<ArgsType>(args)...));
  }
  /**
   * Mark this entity to be removed from the world.
   */
  void kill();
  bool is_dead() const;
  /**
   * Unless this is called, the entity can not be selected or given orders
   * by any player (for example projectiles, ground AOEs etc)
   */
  void make_manipulable();
  bool is_manipulable() const;

private:
  Entity& operator=(const Entity&) = delete;
  Entity& operator=(Entity&&) = delete;
  Entity(const Entity&) = delete;
  Entity(Entity&&) = delete;

public:
  /**
   * Incremented each type we create a new entity, and used as Entity::id.
   */
  static EntityId current_id;
  /**
   * An uniq id for the entity.
   */
  EntityId id;
  /**
   * The type of the entity
   */
  EntityType type;
  /**
   * Whether or not the entity should be deleted from the World on the next
   * cleanup.
   */
  bool to_be_deleted;
  /**
   * The entity name
   */
  std::string name;
  /**
   * Whether or not this entity can be manipulated by a player (and thus
   * selected)
   */
  bool manipulable;
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
   * and pop_front instead. Should be almost as efficient.
   */
  std::list<std::unique_ptr<Work>> works;
  /**
   * A map of components that define what the entity can do. Without a
   * component, it's basically empty and can do nothing in the world.
   *
   * For example an entity that has a world position and can move has a
   * PositionComponent, which tracks the position of the entity and has
   * methods to alter this position.
   */
  std::array<std::unique_ptr<Component>, ComponentTypeCount> components;
  /**
   * The list of Status that currently affect this entity.
   */
  std::vector<std::unique_ptr<Status>> status;
};

#endif // __ENTITY_HPP__
