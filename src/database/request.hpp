/** @addtogroup User
 *  @{
 */

/**
 * Contains all Requests query
 * @class Request
 */

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <database/db_object.hpp>

class Request: public DbObject
{
public:
  Request();
  ~Request();
  /**
   * Get a Request
   * @param sender The request sender
   * @param receiver The request receiver
   * @return DbObject*
   */
  DbObject* get(const DbObject*, const DbObject*);
  /**
   * Create a Request
   * @param id The friend id.
   * @return void
   */
  void create(const DbObject*, const DbObject*);
  /**
   * Accept the request
   * @param id The friend id.
   * @return void
   */
  virtual void accept(const DbObject*, const DbObject*) = 0;
  /**
   * Deny the request
   * @param id The friend id.
   * @return void
   */
  void deny(const DbObject*, const DbObject*);
protected:
  std::string request_id;
};

#endif
/**@}*/
