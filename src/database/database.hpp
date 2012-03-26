/** @addtogroup Database
 *  @{
 */

/**
 * Database Manager
 * @class Database
 */

#ifdef WIN32
# include <winsock.h>
#endif /* WIN32 */


#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <database/db_object.hpp>

#ifndef __DATABASE_HPP__
# define __DATABASE_HPP__

/**
 * @def DB_PORT
 * The database port.
 */
# define DB_PORT 0
/**
 * @def DB_UNIX_SOCKET
 * The database unix socket
 */
# define DB_UNIX_SOCKET NULL
/**
 * @def DB_CLIENT_FLAG
 * specifiy the database port
 */
# define DB_CLIENT_FLAG 0

class Database
{
public:
  ~Database();
  /**
   * Create a single instance of Database.
   * @return Database*
   */
  static Database* inst();
  /**
   * Get an object of the database. If the SQL query would return more than one
   * row, you should use get_objects instead.
   * If there’s no row, it returns NULL.
   * @param columns The database columns to fetch.
   * @param table   The database table to query.
   * @param where   The where query.
   * @return DbObject*
  */
  DbObject* get_object(const std::string&, const std::string&, const std::string&) const;
  /**
   * Get a list of objects of the database.
   * If no row matches, the returned vector is empty, otherwise it contains
   * one or more objects.
   * @param columns The database columns to fetch.
   * @param table   The database table to query.
   * @param where   The where query.
   * @return std::vector<DbObject*>
  */
  std::vector<DbObject*> get_objects(const std::string&, const std::string&, const std::string&) const;
  /**
   * Update a database object (insert it if it doesn’t already exist
   * or create it if it’s new).
   * @return bool
   */
  bool const update(const DbObject* object, const std::string&) const;
  /**
   * Remove a database object.
   * @param query The remove query.
   * @param where The remove query condition.
   * @return bool
   */
  bool const remove(const std::string&, const std::string&) const;

private:
  /**
   * Do a SELECT query on the database and get the results. Returns NULL if an
   * error occured or if there are no result.
   * @param query The SQL query.
   * @return MYSQL_RES*
   */
  MYSQL_RES* do_query(const std::string&) const;
  /**
   * Do a UPDATE or INSERT query on the database.
   * @param query The SQL query.
   * @return bool
   */
  bool do_update(const std::string&) const;
  /**
   * Do a DELETE query on the database.
   * @param query The SQL query.
   * @return bool
   */
  bool do_remove(const std::string&) const;
  Database();
  void connect();
  void close();

  static Database* instance;
  MYSQL *mysql;
};
#endif
/**@}*/
