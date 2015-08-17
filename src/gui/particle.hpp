#ifndef PARTICLE_HPP_INCLUDED
#define PARTICLE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <utils/time.hpp>

#include <logging/logging.hpp>

using namespace std::chrono_literals;

class Particle
{
  friend class ParticleEffect;
  friend class Explosion;
  friend class SmokeTrail;

public:
  Particle(const Particle&) = default;
  Particle(Particle&&) = default;
  Particle& operator=(const Particle&) = default;
  Particle& operator=(Particle&&) = default;

  Particle(const utils::Duration& lifetime):
    lifetime(lifetime),
    life(lifetime),
    sprite{},
    position{},
    velocity{},
    rotation(0.f),
    rotation_speed(0.f),
    friction(0.f),
    gravity(0.f),
    start_size(1.f),
    end_size(0.f)
  {}
  ~Particle() = default;
  void update(const utils::Duration& dt)
  {
    this->life -= dt;

    const float fsec = utils::sec(dt).count();

    this->position += this->velocity * fsec;
    this->rotation += this->rotation_speed * fsec;

    this->velocity.y += this->gravity * fsec;
    this->velocity *= 1.0f - (this->friction * fsec);
    this->rotation_speed *= this->friction * fsec;
  }

  void draw(sf::RenderTarget& surface, const sf::RenderStates& states) const
  {
    auto plife = static_cast<float>(this->life.count()) / this->lifetime.count();
    auto size = this->end_size + ((this->start_size - this->end_size) * plife);
    this->sprite.setScale(size, size);
    this->sprite.setPosition(this->position);
    surface.draw(this->sprite, states);
  }

private:
  utils::Duration lifetime;
  utils::Duration life;
  mutable sf::Sprite sprite;
  sf::Vector2<float> position;
  sf::Vector2<float> velocity;
  float rotation;
  float rotation_speed;
  float friction;
  float gravity;
  float start_size;
  float end_size;
};

#endif /* PARTICLE_HPP_INCLUDED */
