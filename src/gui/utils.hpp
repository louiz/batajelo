#ifndef GUI_COLORS_HPP_INCLUDED
#define GUI_COLORS_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <random>
/**
 * Mix two colors to create a new color. The coef arg tels us how much of
 * the second color there is. If the coef is 1.0, the color will be the 2nd,
 * if it’s 0.0 it will be the first, and in between, it will mix
 * approprietely.
 */
namespace gui
{
  sf::Color mix(const sf::Color& first, const sf::Color& second,
                const float coef);
  float frand();
  sf::Vector2<float> random_dir();
  std::mt19937& rng();
}

#endif /* GUI_COLORS_HPP_INCLUDED */
