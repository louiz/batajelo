/** @addtogroup Database
 *  @{
 */

/**
 * Contains all informations, relations of an User
 * @class User
 */

#include <vector>

#include <database/db_object.hpp>

#ifndef __USER_HPP__
# define __USER_HPP__

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
   * Add a friend.
   * @param id The friend id.
   * @return void
   */
  void add_friend(const std::string&);
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
   * User values
   */
  std::map<std::string, std::string> values;

private:
  User(const DbObject&);
};

#endif
/**@}*/
