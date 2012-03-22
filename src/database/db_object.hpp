/** @addtogroup Database
 *  @{
 */

/**
 * Database Object
 * @class DbObject
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "boost/date_time/posix_time/posix_time.hpp"

#ifndef __DB_OBJECT_HPP__
# define __DB_OBJECT_HPP__

class DbObject
{
public:
  DbObject();
  ~DbObject();
  /**@{*/
  /**
   * @name Getters & setters
   */
  /**
   * Get the field value.
   * @param const std::string& field
   * @return const std::string
   */
  const std::string get(const std::string&) const;
  /**
   * Get the integer field value.
   * @param std::string& field
   * @return const int
   */
  const int get_int(const std::string&) const;
  /**
   * Get the date field value.
   * @param std::string& field The field
   * @return const boost::posix_time::ptime
   */
  boost::posix_time::ptime get_date(const std::string&) const;
  /**
   * Set the field value.
   * @param const std::string& field
   * @param const std::string& value
   * @return void
   */
  void set(const std::string&, const std::string&);
  /**@}*/
  /**@{*/
  /**
   * @name Printers
   */
  /**
   * Print the current map values.
   * @return void
   */
  void print();
  /**
   * @todo Create a print for vector.
   * @return void
   */
  template<typename T>
  void print_vector(std::vector<T*>);
  /**@}*/
  /**
   * Database object values.
   */
  std::map<std::string, std::string> values;

  virtual const std::string get_class_name() const;
};

#endif
/**@}*/
