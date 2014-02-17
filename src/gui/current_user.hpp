/** @addtogroup Ui
 *  @{
 */

/**
 * CurrentUser
 * @class CurrentUser
 */

#include <map>

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

private:
  std::map<std::string, std::string> current_user;
};

#endif // __CURRENT_USER_HPP__
/**@}*/
