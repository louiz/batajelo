/** @addtogroup World
 *  @{
 */

/**
 * Represents an other user who joined the game.
 * It can be a Player or a Viewer.
 * @class Occupant
 */

#ifndef __OCCUPANT_HPP__
# define __OCCUPANT_HPP__

#include <logging/logging.hpp>
#include <serialization/serializable.hpp>

class Occupant: public Serializable
{
public:
  Occupant(unsigned long int number, const std::string& name);
  Occupant();
  ~Occupant();

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & this->number & this->name & this->team;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
    ar & this->number & this->name & this->team;
  }

  unsigned long int number;
  unsigned int team;
  std::string name;

private:
  Occupant(const Occupant&);
  Occupant& operator=(const Occupant&);
};

#endif // __OCCUPANT_HPP__
