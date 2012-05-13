#ifndef __PLAYER_HPP__
# define __PLAYER_HPP__

#include <world/occupant.hpp>

class Player: public Occupant
{
public:
  Player(unsigned long int number, const std::string& name);
  Player();
  ~Player();

  virtual void serialize(oarchive & ar, const unsigned int version)
  {
    Occupant::serialize(ar, version);
  }
  virtual void serialize(iarchive & ar, const unsigned int version)
  {
    Occupant::serialize(ar, version);
  }

private:
  Player(const Player&);
  Player& operator=(const Player&);
};

#endif // __PLAYER_HPP__
