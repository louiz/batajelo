#include <gui/sprites/pic_sprite.hpp>
#include <logging/logging.hpp>
#include <gui/camera/camera.hpp>
#include <game/game_client.hpp>
#include <world/health.hpp>
#include <world/manapool.hpp>
#include <world/team.hpp>
#include <world/location.hpp>

PicpicSprite::PicpicSprite(const Entity* const entity):
  EntitySprite(entity),
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

  Health* entity_health = entity->get<Health>();
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

}

void PicpicSprite::tick()
{
  if (this->height > 2 || this->height < -5)
    this->float_direction = -this->float_direction;
  this->height += this->float_direction;
}

bool PicpicSprite::init = false;
sf::Texture PicpicSprite::body_texture;
sf::Texture PicpicSprite::eye_texture;
