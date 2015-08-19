#ifndef SIMPLE_WORLD_SPRITE_HPP_INCLUDED
#define SIMPLE_WORLD_SPRITE_HPP_INCLUDED

#include <gui/sprites/world_sprite.hpp>

class SimpleWorldSprite: public WorldSprite
{
public:
  SimpleWorldSprite(const Position& position, const sf::Sprite& sprite):
    position(position),
    sprite(sprite)
  {}
  ~SimpleWorldSprite() = default;
  Position get_world_pos() const
  {
    return this->position;
  }
  void draw(sf::RenderTarget& surface, const sf::RenderStates& states) const
  {
    surface.draw(this->sprite, states);
  }

private:
  const Position position;
  const sf::Sprite& sprite;

  SimpleWorldSprite(const SimpleWorldSprite&) = delete;
  SimpleWorldSprite(SimpleWorldSprite&&) = delete;
  SimpleWorldSprite& operator=(const SimpleWorldSprite&) = delete;
  SimpleWorldSprite& operator=(SimpleWorldSprite&&) = delete;
};

#endif /* SIMPLE_WORLD_SPRITE_HPP_INCLUDED */
