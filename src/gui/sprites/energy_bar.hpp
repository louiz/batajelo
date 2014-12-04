#ifndef ENERGY_BAR_HPP_INCLUDE
#define ENERGY_BAR_HPP_INCLUDE

#include <SFML/Graphics.hpp>

/**
 * Contains various details about how an energy bar should be drawn
 */
struct EnergyBar
{
  /**
   * The color of the bar when at its maximum (full)
   */
  sf::Color max_color;
  /**
   * The color of the bar, when at its minimum (empty)
   */
  sf::Color min_color;
  /**
   * Width and height, in pixels
   */
  sf::Vector2f size;
  /**
   * The bar can have vertical lines drawn, every N units. This helps view
   * the scale of the bar, in addition to the percentage of its fullness.
   * Use 0 to disable that kind of line.
   */
  std::size_t little_graduation;
  std::size_t big_graduation;
};

#endif /* ENERGY_BAR_HPP_INCLUDE */
