#include <gui/effects/explosion.hpp>
#include <gui/utils.hpp>
#include <chrono>

using namespace std::chrono_literals;

Explosion::Explosion(const Position& pos, const float height):
  ParticleEffect(pos, 2s, height)
{
  for (auto i = 0; i < 100; ++i)
    {
      auto n = std::uniform_int_distribution<int>(0, 3)(gui::rng());
      Particle particle(800ms);
      particle.sprite.setTexture(this->texture);
      particle.sprite.setTextureRect({{64 * n, 64},
                                     {64, 64}});
      particle.sprite.setOrigin(64/2, 64/2);
      particle.start_size = 0.3f + gui::frand() * 0.4f;
      auto vel = gui::random_dir();
      vel.x = vel.x * (100 + 300*gui::frand());
      vel.y = vel.y * (100 + 300*gui::frand());
      particle.velocity = vel;
      sf::Color a = sf::Color::Black;
      sf::Color b = sf::Color(120, 120, 120, 200);
      particle.sprite.setColor(gui::mix(a, b, gui::frand()));
      particle.friction = 8.f;
      particle.gravity = -600.f;
      this->particles.push_back(std::move(particle));
    }
}
