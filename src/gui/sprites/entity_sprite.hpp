#ifndef __ENTITY_SPRITE_HPP__
# define __ENTITY_SPRITE_HPP__

#include <gui/sprites/world_sprite.hpp>
#include <gui/camera/camera.hpp>

#include <vector>

enum class TaskType;
class Task;
class Entity;

class EntitySprite: public WorldSprite
{
public:
  EntitySprite(const Entity* const entity);
  ~EntitySprite() {}
  const Entity* get_entity() const;
  Position get_world_pos() const override final;
  bool is_mouse_over(const Camera* camera) const;
  /**
   * Set our current animation based on the new task the entity is doing.
   * For example if our new task is TaskType::Move, the current animation we
   * draw (until the task is changed) is the move animation, if any.
   */
  void set_task(const Task* task);

protected:
  const Entity* const entity;
  void draw_shadow(Camera& camera, const sf::Color color) const;
  virtual void on_task_changed(const Task*) {}

  static const std::vector<sf::Color> team_colors;
  TaskType task_type;

private:
  static bool init;
  static sf::Texture shadow_texture;

  EntitySprite(const EntitySprite&);
  EntitySprite& operator=(const EntitySprite&);
};

#endif // __ENTITY_SPRITE_HPP__
