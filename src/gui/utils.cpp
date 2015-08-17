#include <gui/utils.hpp>
#include <utils/math.hpp>
#include <cstdint>

namespace gui
{
sf::Color mix(const sf::Color& first, const sf::Color& second,
              const float coef)
{
  uint16_t r = ((first.r * (1 - coef)) + (second.r * coef));
  uint16_t g = ((first.g * (1 - coef)) + (second.g * coef));
  uint16_t b = ((first.b * (1 - coef)) + (second.b * coef));
  uint16_t a = ((first.a * (1 - coef)) + (second.a * coef));
  return {static_cast<uint8_t>(r),
      static_cast<uint8_t>(g),
      static_cast<uint8_t>(b),
      static_cast<uint8_t>(a)};

}

std::mt19937& rng()
{
  static std::random_device rd;
  static std::mt19937 gen(rd());

  return gen;
}

float frand()
{
  return std::uniform_real_distribution<float>(0, 1)(rng());
}

sf::Vector2<float> random_dir()
{
  return normalize(sf::Vector2<float>(frand() - 0.5f, frand() - 0.5f));
}

} // namespace gui
