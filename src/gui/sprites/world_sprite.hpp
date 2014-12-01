#ifndef __SPRITE_HPP__
# define __SPRITE_HPP__

#include <SFML/Graphics.hpp>
#include <chrono>

#include <world/position.hpp>

class GameClient;

class WorldSprite
{
public:
  WorldSprite() {}
  virtual ~WorldSprite() {}
  virtual void draw(GameClient* game) const = 0;
  virtual void tick() = 0;
  virtual Position get_world_pos() const = 0;
private:
  WorldSprite(const WorldSprite&);
  WorldSprite& operator=(const WorldSprite&);
};

#endif // __SPRITE_HPP__
