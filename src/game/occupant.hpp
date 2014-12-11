/**
 * Represents an other user who joined the game.
 * It can be a Player or a Viewer.
 * @class Occupant
 */

#ifndef __OCCUPANT_HPP__
# define __OCCUPANT_HPP__

#include <cstdint>

#include "game.pb.h"

class Occupant
{
public:
  Occupant(uint32_t id, const std::string& name);
  Occupant(const ser::game::Occupant& srl);
  ~Occupant();

  void set_team(const uint16_t team);
  uint16_t get_team() const;

  ser::game::Occupant serialize() const;

  uint32_t id;
  uint16_t team;
  std::string name;

private:
  Occupant(const Occupant&);
  Occupant& operator=(const Occupant&);
};

#endif // __OCCUPANT_HPP__
