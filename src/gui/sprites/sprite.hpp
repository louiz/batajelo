#ifndef __SPRITE_HPP__
# define __SPRITE_HPP__

#include <SFML/Graphics.hpp>
#include <chrono>

#include <gui/camera/camera.hpp>

class ClientWorld;
class Screen;

class Sprite
{
public:
  Sprite() {}
  virtual ~Sprite() {}
  virtual void draw(Camera*, const ClientWorld*, const Screen*) const = 0;
  virtual void tick() = 0;
private:
  Sprite(const Sprite&);
  Sprite& operator=(const Sprite&);
};

#endif // __SPRITE_HPP__
