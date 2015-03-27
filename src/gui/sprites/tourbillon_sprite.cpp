#include <gui/sprites/tourbillon_sprite.hpp>
#include <logging/logging.hpp>
#include <gui/camera/camera.hpp>
#include <game/game_client.hpp>
#include <world/health.hpp>
#include <world/manapool.hpp>
#include <world/team.hpp>
#include <world/location.hpp>

TourbillonSprite::TourbillonSprite(const Entity* const entity):
  EntitySprite(entity)
{
  if (TourbillonSprite::init == false)
    {
      if (!TourbillonSprite::body_texture.loadFromFile("data/images/units/tourbillon.png"))
        exit(1);
      TourbillonSprite::body_texture.setSmooth(true);
      TourbillonSprite::init = true;
    }
}

void TourbillonSprite::draw(GameClient* game) const
{
  Team* team = this->entity->get<Team>();
  assert(team);
  Location* location = this->entity->get<Location>();
  assert(location);
  this->draw_shadow(game->get_camera(), this->team_colors[team->get()]);
  const auto entpos = game->get_camera().world_to_camera_position(location->position());

  const float x = entpos.x - game->get_camera().x;
  const float y = entpos.y - game->get_camera().y;

  sf::Sprite sprite(TourbillonSprite::body_texture);
  const sf::Vector2u size = TourbillonSprite::body_texture.getSize();
  sprite.setPosition(x - size.x/2, y - size.y);
  game->get_camera().draw(sprite);

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

void TourbillonSprite::tick()
{
}

bool TourbillonSprite::init = false;
sf::Texture TourbillonSprite::body_texture;
