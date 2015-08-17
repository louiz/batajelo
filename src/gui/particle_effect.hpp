#ifndef PARTICLE_EFFECT_HPP_INCLUDED
#define PARTICLE_EFFECT_HPP_INCLUDED

#include <gui/effect.hpp>
#include <gui/particle.hpp>
#include <vector>
#include <chrono>

using namespace std::chrono_literals;

/**
 * An Effect that contains a list of particles
 */

class ParticleEffect: public Effect
{
public:
  ParticleEffect(const Position& pos, const utils::Duration& lifetime):
    ParticleEffect(pos, lifetime, 0.f)
  {}
  ParticleEffect(const Position& pos, const utils::Duration& lifetime, const float height):
    Effect(pos, lifetime),
    height(height)
  {}
  ~ParticleEffect() = default;

  void update(const utils::Duration& dt) override final
  {
    for (auto it = this->particles.begin(); it != this->particles.end(); )
      {
        it->update(dt);
        if (it->life <= 0s)
          it = this->particles.erase(it);
        else
          ++it;
      }
    Effect::update(dt);
  }

  void draw(sf::RenderTarget& surface, const sf::RenderStates& states) const override final
  {
    auto hstates = states;
    hstates.transform.translate(0, -this->height);

    for (auto& particle: this->particles)
      particle.draw(surface, hstates);
  }

  static sf::Texture texture;

protected:
  std::vector<Particle> particles;
  const float height;

  ParticleEffect(const ParticleEffect&) = delete;
  ParticleEffect(ParticleEffect&&) = delete;
  ParticleEffect& operator=(const ParticleEffect&) = delete;
  ParticleEffect& operator=(ParticleEffect&&) = delete;
};

#endif /* PARTICLE_EFFECT_HPP_INCLUDED */
