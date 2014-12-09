#include <gui/sprites/pic_sprite.hpp>
#include <logging/logging.hpp>
#include <gui/camera/camera.hpp>
#include <game/game_client.hpp>
#include <world/unit.hpp>
#include <world/health.hpp>
#include <world/mana.hpp>

PicpicSprite::PicpicSprite(const Unit* const unit):
  UnitSprite(unit),
  float_direction(-0.0002)
{
  if (PicpicSprite::init == false)
    {
      if (!PicpicSprite::body_texture.loadFromFile("data/images/units/picpic.svg.png"))
        exit(1);
      if (!PicpicSprite::eye_texture.loadFromFile("data/images/units/picpic_eye.svg.png"))
        exit(1);
      PicpicSprite::init = true;
    }
  this->height = -5 + (7 * random() / RAND_MAX);
}

void PicpicSprite::draw(GameClient* game) const
{
  this->draw_shadow(game->get_camera());
  const auto entpos = game->get_camera().world_to_camera_position(this->unit->pos);

  const int x = entpos.x - game->get_camera().x;
  const int y = entpos.y - game->get_camera().y;

  sf::Sprite sprite(PicpicSprite::body_texture);
  const sf::Vector2u size = PicpicSprite::body_texture.getSize();
  sprite.setPosition(x - size.x/2, y - size.y - this->height);
  game->get_camera().draw(sprite);

  sf::Sprite eye_sprite(PicpicSprite::eye_texture);
  const sf::Vector2u eye_size = PicpicSprite::eye_texture.getSize();
  eye_sprite.setPosition(x - 10 - eye_size.x/2, y - size.y / 5 * 3 - this->height - eye_size.y/2 + 5);
  sf::Vector2i mouse_pos = game->get_screen().get_mouse_position();
  game->get_camera().draw(eye_sprite);
  eye_sprite.setPosition(x + 10 - eye_size.x/2, y - size.y / 5 * 3 - this->height - eye_size.y/2 + 5);
  game->get_camera().draw(eye_sprite);

  EnergyBar bar = this->standard_health_bar;

  Health* unit_health = unit->get<Health>();
  if (unit_health)
    {
      game->get_debug_hud().add_debug_line("Unit health: " + std::to_string(unit_health->get()) + "/" +std::to_string(unit_health->get_max()));
      game->get_camera().draw_energy_bar({x, y - 90}, bar, unit_health->get_max().to_int(), unit_health->get().to_int());
    }
  EnergyBar mana_bar = this->standard_mana_bar;
  Mana* unit_mana = unit->get<Mana>();
  if (unit_mana)
    {
      game->get_debug_hud().add_debug_line("Unit mana: " + std::to_string(unit_mana->get()) + "/" +std::to_string(unit_mana->get_max()));
      game->get_camera().draw_energy_bar({x, y - 80}, mana_bar, unit_mana->get_max().to_int(), unit_mana->get().to_int());
    }

}

void PicpicSprite::tick()
{
  if (this->height > 6 || this->height < 1)
    this->float_direction = -this->float_direction;
  this->height += this->float_direction;
}

bool PicpicSprite::init = false;
sf::Texture PicpicSprite::body_texture;
sf::Texture PicpicSprite::eye_texture;
