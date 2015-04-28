#ifndef ANIMATIONS_HPP_INCLUDED
#define ANIMATIONS_HPP_INCLUDED

#include <gui/sprites/energy_bar.hpp>

struct AttackAnimation
{
  /**
   * The total duration of the frontswing
   */
  std::size_t fs;
  /**
   * The total duration of the backswing
   */
  std::size_t bs;
};

#endif /* ANIMATIONS_HPP_INCLUDED */
