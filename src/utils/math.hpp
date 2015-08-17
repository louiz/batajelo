#ifndef MATH_HPP_INCLUDED
#define MATH_HPP_INCLUDED

#include <SFML/System/Vector2.hpp>

template<typename T>
inline sf::Vector2<T> normalize(const sf::Vector2<T>& v)
{
  T l = static_cast<T>(1.0f/::sqrtf(v.x*v.x + v.y*v.y));
  return sf::Vector2<T>(v.x*l, v.y*l);
}

#endif /* MATH_HPP_INCLUDED */
