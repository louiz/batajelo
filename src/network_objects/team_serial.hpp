/** @addtogroup Network Objects
 *  @{
 */

/**
 * Represents the teams of an user.
 * @class TeamsUserSerial
 */

#ifndef __TEAM_SERIAL_HPP__
# define __TEAM_SERIAL_HPP__

#include <serialization/serializable.hpp>
#include <network_objects/team.hpp>
#include <database/db_object.hpp>

class TeamSerial: public Serializable
{
public:
  TeamSerial(const std::vector<DbObject*>&);
  TeamSerial();
  virtual ~TeamSerial();

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & this->teams;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
    ar & this->teams;
  }

  std::vector<Team*> teams;
private:
  TeamSerial(const TeamSerial&);
  TeamSerial& operator=(const TeamSerial&);
};

#endif // __TEAM_SERIAL_HPP__
