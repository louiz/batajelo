#include <gui/sprites/emp_sprite.hpp>
#include <world/location.hpp>
#include <world/entity.hpp>

constexpr unsigned int emp_radius = 300;

EmpSprite::EmpSprite(const Entity* const entity):
  EntitySprite(entity),
  circle(emp_radius)
{
  this->circle.setOrigin(emp_radius, emp_radius);
  this->circle.setScale(1, 0.666f);
  this->circle.setFillColor({255, 0, 255, 123});
}

void EmpSprite::draw(sf::RenderTarget& surface, const sf::RenderStates& states) const
{
  this->circle.setPosition(0, 0);
  surface.draw(this->circle, states);
}

void EmpSprite::tick()
{
}
