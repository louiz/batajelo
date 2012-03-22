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
   * Get an object of the database.
   * @param const std::string& columns The database columns to fetch.
   * @param const std::string& table   The database table to query.
   * @param const std::string& where   The where query.
   * @retval DbObject*
  */
  DbObject* get_object(const std::string&, const std::string&, const std::string&) const;
  /**
   * Get objects of the database.
   * @param const std::string&  columns
   * @param const std::string&  table
   * @param const std::string&  where
   * @return std::vector<DbObject*>
  */
  std::vector<DbObject*> get_objects(const std::string&, const std::string&, const std::string&) const;
  /**
   * Query the database.
   * @param const std::string& query
   * @return MYSQL_RES*
   */
  MYSQL_RES* do_query(const std::string&) const;
  /**
   * @todo Update a database object.
   * @return void
   */
  void update(std::string*&);
  /**
   * @todo Insert a database object.
   * @return void
   */
  void insert();

private:
  Database();
  void connect();
  DbObject query_object(std::string const&, bool);
  void close();

  static Database* instance;
  MYSQL *mysql;
};
#endif
/**@}*/
