#include <gui/sprites/pic_sprite.hpp>
#include <logging/logging.hpp>
#include <gui/camera/camera.hpp>
#include <game/game_client.hpp>
#include <world/health.hpp>
#include <world/manapool.hpp>
#include <world/team.hpp>
#include <world/location.hpp>
#include <world/tasks/attack_task.hpp>

PicpicSprite::PicpicSprite(const Entity* const entity):
  EntitySprite(entity),
  float_direction(-0.0002),
  animation(PicpicSprite::Animation::Idle)
{
  if (PicpicSprite::init == false)
    {
      if (!PicpicSprite::body_texture.loadFromFile("data/images/units/picpic.svg.png"))
        exit(1);
      if (!PicpicSprite::eye_texture.loadFromFile("data/images/units/picpic_eye.svg.png"))
        exit(1);
      PicpicSprite::body_texture.setSmooth(true);
      PicpicSprite::eye_texture.setSmooth(true);
      PicpicSprite::init = true;
    }
  this->height = -5 + (7 * random() / RAND_MAX);
}

void PicpicSprite::draw(sf::RenderTarget& surface, const sf::RenderStates& states) const
{
  sf::Sprite sprite(PicpicSprite::body_texture);
  const sf::Vector2u size = PicpicSprite::body_texture.getSize();
  sprite.setOrigin(size.x/2, size.y);
  surface.draw(sprite, states);

  switch (this->animation)
    {
    case Animation::Idle:
      return;
    case Animation::Move:
      // TODO
      break;
    case Animation::Attack:
      this->draw_attack_animation(surface, this->entity->get_task<AttackTask>());
      break;
    }
}

void PicpicSprite::tick()
{
  if (this->height > 2 || this->height < -5)
    this->float_direction = -this->float_direction;
  this->height += this->float_direction;
}

void PicpicSprite::on_task_changed(const Task* task)
{
  log_debug("on task change (init): " << static_cast<int>(task->get_type()));
  switch (task->get_type())
    {
    case TaskType::None:
      this->animation = PicpicSprite::Animation::Idle;
      break;
    case TaskType::Attack:
      this->animation = PicpicSprite::Animation::Attack;
      this->init_attack_animation(static_cast<const AttackTask*>(task));
      break;
    case TaskType::Follow:
    case TaskType::Path:
      this->animation = PicpicSprite::Animation::Move;
      break;
    }
}

void PicpicSprite::init_attack_animation(const AttackTask* task)
{
  log_debug("Init task anim with: fs=" << task->get_remaining_frontswing_duration() <<
            " and bs=" << task->get_remaining_backswing_duration());
  this->attack_animation = {task->get_remaining_frontswing_duration(),
                            task->get_remaining_backswing_duration()};
}

void PicpicSprite::draw_attack_animation(sf::RenderTarget& surface, const AttackTask* task) const
{
  std::size_t current_fs;
  std::size_t current_bs;

  if (task)
    {
      current_fs = task->get_remaining_frontswing_duration();
      current_bs = task->get_remaining_backswing_duration();
    }
  else
    {
      current_fs = 0;
      current_bs = this->attack_animation.bs;
    }

  auto current_value = (current_fs > 0 ? this->attack_animation.fs - current_fs: current_bs);
  int max = (current_fs > 0 ? this->attack_animation.fs : this->attack_animation.bs);

  EnergyBar bar = {
    sf::Color::Red,
    sf::Color::Red,
    {8, 100},
    2,
    0
  };
  // game->get_camera().draw_vertical_bar({-80.f, 0.f}, bar, max, current_value);
}

bool PicpicSprite::init = false;
sf::Texture PicpicSprite::body_texture;
sf::Texture PicpicSprite::eye_texture;
