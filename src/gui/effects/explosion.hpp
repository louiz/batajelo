#ifndef EXPLOSION_HPP_INCLUDED
#define EXPLOSION_HPP_INCLUDED

#include <gui/particle_effect.hpp>

class Explosion: public ParticleEffect
{
public:
  Explosion(const Position& pos, const float height);
  ~Explosion() = default;

private:
  Explosion(const Explosion&) = delete;
  Explosion(Explosion&&) = delete;
  Explosion& operator=(const Explosion&) = delete;
  Explosion& operator=(Explosion&&) = delete;
};


#endif /* EXPLOSION_HPP_INCLUDED */
