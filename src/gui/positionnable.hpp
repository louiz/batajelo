#ifndef POSITIONNABLE_HPP_INCLUDED
#define POSITIONNABLE_HPP_INCLUDED

#include <world/position.hpp>

class Positionnable
{
public:
  Positionnable(const Position& pos):
    position(pos)
  {}
  ~Positionnable() = default;

protected:
  Position position;

private:
  Positionnable(const Positionnable&) = delete;
  Positionnable(Positionnable&&) = delete;
  Positionnable& operator=(const Positionnable&) = delete;
  Positionnable& operator=(Positionnable&&) = delete;
};


#endif /* POSITIONNABLE_HPP_INCLUDED */
