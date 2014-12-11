#include <gui/sprites/unit_sprite.hpp>
#include <world/unit.hpp>

sf::Texture UnitSprite::shadow_texture;
bool UnitSprite::init = false;
const std::vector<sf::Color> UnitSprite::team_colors = {sf::Color::White,
                                                        sf::Color::Red,
                                                        sf::Color::Blue};

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

void UnitSprite::draw_shadow(Camera& camera, const sf::Color color) const
{
  const auto entpos = camera.world_to_camera_position(this->unit->pos);
  const int x = entpos.x - camera.x;
  const int y = entpos.y - camera.y;
  const sf::Vector2u size = UnitSprite::shadow_texture.getSize();
  sf::Sprite sprite(UnitSprite::shadow_texture);
  sprite.setPosition(x - size.x/2, y - size.y/2);

  sprite.setColor(color);
  camera.draw(sprite);
}

const Unit* const UnitSprite::get_unit() const
{
  return this->unit;
}

Position UnitSprite::get_world_pos() const
{
  return this->unit->pos;
}
