#ifndef __ENTITY_SPRITE_HPP__
# define __ENTITY_SPRITE_HPP__

#include <gui/sprites/world_sprite.hpp>
#include <gui/camera/camera.hpp>

#include <vector>

class Entity;

class EntitySprite: public WorldSprite
{
public:
  EntitySprite(const Entity* const entity);
  ~EntitySprite() {}
  const Entity* get_entity() const;
  Position get_world_pos() const override final;
  bool is_mouse_over(const Camera* camera) const;

protected:
  const Entity* const entity;
  void draw_shadow(Camera& camera, const sf::Color color) const;

  static const std::vector<sf::Color> team_colors;

private:
  static bool init;
  static sf::Texture shadow_texture;

  EntitySprite(const EntitySprite&);
  EntitySprite& operator=(const EntitySprite&);
};

#endif // __ENTITY_SPRITE_HPP__
