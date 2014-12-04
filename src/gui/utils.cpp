#include <gui/utils.hpp>
#include <cstdint>

sf::Color mix(const sf::Color& first, const sf::Color& second,
              const float coef)
{
  uint16_t r = ((first.r * (1 - coef)) + (second.r * coef));
  uint16_t g = ((first.g * (1 - coef)) + (second.g * coef));
  uint16_t b = ((first.b * (1 - coef)) + (second.b * coef));
  uint16_t a = ((first.a * (1 - coef)) + (second.a * coef));
  return {r, g, b, a};
}
