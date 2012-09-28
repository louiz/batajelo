/** @addtogroup Database
 *  @{
 */

/**
 * Contains all informations, relations of a Team
 * @class Team
 */

#include <vector>

#ifndef __TEAM_HPP__
# define __TEAM_HPP__

#include <database/db_object.hpp>

class Team: public DbObject
{
public:
  Team();
  ~Team();
private:
  /**
   * Add a team.
   * @return void
   */
  void add_team();
  /**
   * Add a user to a team
   * @param mode The game mode
   * @param race The race played
   * @return void
   */
  void add_user(const std::string&, const std::string&);
};

#endif
/**@}*/