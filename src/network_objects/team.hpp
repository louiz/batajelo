/** @addtogroup Network Objects
 *  @{
 */

/**
 * Represents the infos about a team.
 * @class Team
 */

#ifndef __TEAM_HPP__
# define __TEAM_HPP__

#include <logging/logging.hpp>
#include <serialization/serializable.hpp>
#include <network_objects/player.hpp>
#include <database/db_object.hpp>

class Team: public Serializable 
{
public:
  Team();
  Team(const std::string&, const std::string&, const std::string&, 
       const std::string&, const std::string&, const std::string&,
       const std::vector<Player*>&);
  virtual ~Team();

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & this->id & this->mode & this->ladder & this->season & this->win & this->loss & this->mates;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
   ar & this->id & this->mode & this->ladder & this->season & this->win & this->loss & this->mates;
  }

  std::string id;
  std::string mode;
  std::string ladder;
  std::string season;
  std::string win;
  std::string loss; 
  std::vector<Player*> mates;
  
private:
  Team(const Team&);
  Team& operator=(const Team&);
};

#endif // __TEAM_HPP__
