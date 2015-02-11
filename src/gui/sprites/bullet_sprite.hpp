#ifndef BULLET_SPRITE_HPP_INCLUDED
#define BULLET_SPRITE_HPP_INCLUDED

#include <gui/sprites/entity_sprite.hpp>

class BulletSprite: public EntitySprite
{
public:
  BulletSprite(const Entity* const);
  ~BulletSprite() = default;
  void draw(GameClient* game) const override final;
  void tick() override final;
  static bool init;

private:

  static sf::Texture texture;

  BulletSprite(const BulletSprite&) = delete;
  BulletSprite(BulletSprite&&) = delete;
  BulletSprite& operator=(const BulletSprite&) = delete;
  BulletSprite& operator=(BulletSprite&&) = delete;
};



#endif /* BULLET_SPRITE_HPP_INCLUDED */
