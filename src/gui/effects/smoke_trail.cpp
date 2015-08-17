#include <gui/effects/smoke_trail.hpp>
#include <gui/utils.hpp>
#include <utils/math.hpp>
#include <chrono>

using namespace std::chrono_literals;

SmokeTrail::SmokeTrail(const Position& pos, const float height):
  ParticleEffect(pos, 1s, height)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  auto n = std::uniform_int_distribution<int>(0, 3)(gui::rng());
  Particle particle(800ms);
  particle.sprite.setTexture(this->texture);
  particle.sprite.setTextureRect({{64 * n, 64},
        {64, 64}});
  particle.sprite.setOrigin(64/2, 64/2);
  particle.start_size = 0.2f + gui::frand() * 0.5f;
  particle.velocity.x = 0;
  particle.velocity.y = -(500 + 1000*gui::frand());
  particle.sprite.setColor(gui::mix(sf::Color(120, 120, 120, 200),
                                    sf::Color::White, gui::frand()));
  particle.friction = 10.f;
  particle.gravity = -300.f;
  this->particles.push_back(std::move(particle));
}
