/** @addtogroup Ui
 *  @{
 */

/**
 * CurrentUser
 * @class CurrentUser
 */

#include <map>
#include <network_objects/team_serial.hpp>

#ifndef __CURRENT_USER_HPP__
# define __CURRENT_USER_HPP__

class CurrentUser
{
public:
  CurrentUser();
  ~CurrentUser();
  /**
   * Get the object.
   * @return  const std::map<std::string, std::string>&
   */
  const std::map<std::string, std::string>& get_current_user() const;
  /**
   * Get the field value.
   * @param std::string& field
   * @return const int
   */
  const std::string get_key(const std::string& field) const;
  /**
   * Set the field value.
   * @param const std::string& field
   * @param const std::string& value
   * @return void
   */
  void set_key(const std::string& field, const std::string& value);
  /**
   * Get user teams
   * @return TeamStatsSerial*
   */
  const TeamSerial* get_teams() const;
 /**
   * Set user teams
   * @param TeamStatsSerial* the teams
   * @return TeamStatsSerial*
   */
  void set_teams(TeamSerial*);

private:
  TeamSerial* teams;
  std::map<std::string, std::string> current_user;
};

#endif // __CURRENT_USER_HPP__
/**@}*/
