#ifndef __PICPIC_SPRITE_HPP__
# define __PICPIC_SPRITE_HPP__

#include <gui/sprites/entity_sprite.hpp>
#include <gui/sprites/animations.hpp>

class AttackTask;

class PicpicSprite: public EntitySprite
{
  enum class Animation
  {
    Idle,
    Move,
    Attack,

    count
  };
public:
  PicpicSprite(const Entity* const);

  void draw(GameClient* game) const override final;
  void tick() override final;
  static bool init;

private:
  float height;
  float float_direction;
  Animation animation;

  AttackAnimation attack_animation;

  void on_task_changed(const Task*) override final;
  void init_attack_animation(const AttackTask*);
  void draw_attack_animation(const AttackTask* task, GameClient* game, sf::Vector2f pos) const;

  PicpicSprite(const PicpicSprite&);
  PicpicSprite& operator=(const PicpicSprite&);
  static sf::Texture body_texture;
  static sf::Texture eye_texture;
};

#endif // __PICPIC_SPRITE_HPP__
