#include <gui/sprites/unit_sprite.hpp>
#include <world/unit.hpp>

sf::Texture UnitSprite::shadow_texture;
bool UnitSprite::init = false;

UnitSprite::UnitSprite(const Unit* const unit):
  unit(unit)
{
  if (UnitSprite::init == false)
    {
      if (!UnitSprite::shadow_texture.loadFromFile("data/images/units/shadow.png"))
        exit(1);
      UnitSprite::init = true;
    }
}

void UnitSprite::draw_shadow(Camera& camera) const
{
  const sf::Vector2u entpos = camera.world_to_camera_position(this->unit->pos);
  const uint x = entpos.x - camera.x;
  const uint y = entpos.y - camera.y;
  const sf::Vector2u size = UnitSprite::shadow_texture.getSize();
  sf::Sprite sprite(UnitSprite::shadow_texture);
  sprite.setPosition(x - size.x/2, y - size.y/2);

  const sf::Color color(255, 255, 255, 122);
  sprite.setColor(color);
  camera.draw(sprite);
}
