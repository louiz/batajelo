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
   * Get friends infos.
   * @return std::vector<User*>
   */
  std::vector<User*> get_friends();
  /**
   * @todo use this instead of from ?
   * @return std::string
   */
  std::string get_class_name();
  /**
   * User values
   */
  std::map<std::string, std::string> values;

  virtual const std::string get_class_name() const;

private:
  User(const DbObject&);
};

#endif
/**@}*/
