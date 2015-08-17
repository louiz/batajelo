#ifndef SMOKE_TRAIL_HPP_INCLUDED
#define SMOKE_TRAIL_HPP_INCLUDED

#include <gui/particle_effect.hpp>

class SmokeTrail: public ParticleEffect
{
public:
  SmokeTrail(const Position& pos, const float height);
  ~SmokeTrail() = default;

private:
  SmokeTrail(const SmokeTrail&) = delete;
  SmokeTrail(SmokeTrail&&) = delete;
  SmokeTrail& operator=(const SmokeTrail&) = delete;
  SmokeTrail& operator=(SmokeTrail&&) = delete;
};

#endif /* SMOKE_TRAIL_HPP_INCLUDED */
