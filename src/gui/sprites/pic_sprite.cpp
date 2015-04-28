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

void PicpicSprite::draw(GameClient* game) const
{
  Team* team = this->entity->get<Team>();
  assert(team);
  Location* location = this->entity->get<Location>();
  assert(location);
  this->draw_shadow(game->get_camera(), this->team_colors[team->get()]);
  const auto entpos = game->get_camera().world_to_camera_position(location->position());

  const float x = entpos.x - game->get_camera().x;
  const float y = entpos.y - game->get_camera().y;

  sf::Sprite sprite(PicpicSprite::body_texture);
  const sf::Vector2u size = PicpicSprite::body_texture.getSize();
  sprite.setPosition(x - size.x/2, y - size.y - this->height);
  game->get_camera().draw(sprite);

  sf::Sprite eye_sprite(PicpicSprite::eye_texture);
  const sf::Vector2u eye_size = PicpicSprite::eye_texture.getSize();
  eye_sprite.setPosition(x - 10 - eye_size.x/2, y - size.y / 5 * 3 - this->height - eye_size.y/2 + 5);
  game->get_camera().draw(eye_sprite);
  eye_sprite.setPosition(x + 10 - eye_size.x/2, y - size.y / 5 * 3 - this->height - eye_size.y/2 + 5);
  game->get_camera().draw(eye_sprite);

  EnergyBar bar = this->standard_health_bar;

  Health* entity_health = this->entity->get<Health>();
  if (entity_health)
    {
      game->get_debug_hud().add_debug_line("Entity health: " + std::to_string(entity_health->get()) + "/" +std::to_string(entity_health->get_max()));
      game->get_camera().draw_energy_bar({x, y - 90}, bar, entity_health->get_max().to_int(), entity_health->get().to_int());
    }
  EnergyBar mana_bar = this->standard_mana_bar;
  ManaPool* entity_mana = entity->get<ManaPool>();
  if (entity_mana)
    {
      game->get_debug_hud().add_debug_line("Entity mana: " + std::to_string(entity_mana->get()) + "/" +std::to_string(entity_mana->get_max()));
      game->get_camera().draw_energy_bar({x, y - 80}, mana_bar, entity_mana->get_max().to_int(), entity_mana->get().to_int());
    }

  switch (this->animation)
    {
    case Animation::Idle:
      return;
    case Animation::Move:
      // TODO
      break;
    case Animation::Attack:
      this->draw_attack_animation(this->entity->get_task<AttackTask>(), game, {x - 80, y});
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

void PicpicSprite::draw_attack_animation(const AttackTask* task, GameClient* game, sf::Vector2f pos) const
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
  game->get_camera().draw_vertical_bar(pos, bar, max, current_value);
}

bool PicpicSprite::init = false;
sf::Texture PicpicSprite::body_texture;
sf::Texture PicpicSprite::eye_texture;
