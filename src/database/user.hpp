/** @addtogroup Database
 *  @{
 */

/**
 * Contains all informations, relations of an User
 * @class User
 */

#include <vector>

#ifndef __USER_HPP__
# define __USER_HPP__

#include <database/db_object.hpp>

class User: public DbObject
{
public:
  User();
  ~User();
  /**
   * Add a user achievement.
   * @param id The achievement id.
   * @return void
   */
  void add_achievement(const std::string&);
  /**
   * Get User Achievement.
   * @return std::vector<DbObject*>
   */
  std::vector<DbObject*> get_achievements();
  /**
   * Get friends infos.
   * @return std::vector<User*>
   */
  std::vector<User*> get_friends();
  /**
   * Remove a friend.
   * @param id The friend id.
   * @return void
   */
  void remove_friend(const std::string&);
  /**
   * Link a User to a Replay
   * @param replay_id The Replay id.
   * @return void
   */
  void add_replay(const std::string&);
  /**
   * Get Replays of a User
   * @return std::vector<DbObject*>
   */
  std::vector<DbObject*> get_replays();
  /**
   * Ban a User
   * @param hours The ban length in hours
   * @return void
   */
  void add_ban(const long);
  /**
   * Updte a ban.
   * @param hours The ban length in hours.
   * @return void
   */
  void update_ban(const long);
  /**
   * Remove a ban.
   * @return void
   */
  void remove_ban();
private:
  User(const DbObject&);
};

#endif
/**@}*/
