/** @addtogroup Network Objects
 *  @{
 */

/**
 * Represents the infos about a player.
 * @class Player
 */

#ifndef __PLAYER_HPP__
# define __PLAYER_HPP__

#include <logging/logging.hpp>
#include <serialization/serializable.hpp>

class Player: public Serializable 
{
public:
  Player();
  Player(const std::string&, const std::string&, const std::string&);
  virtual ~Player();

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & this->id & this->login & this->race;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
    ar & this->id & this->login & this->race;
  }

  std::string id;
  std::string login;
  std::string race;
  
private:
  Player(const Player&);
  Player& operator=(const Player&);
};

#endif // __PLAYER_HPP__
