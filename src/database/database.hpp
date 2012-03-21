// Database Implementation

#ifdef WIN32
//socket mysql_real_connect
# include <winsock.h>
#endif /* WIN32 */

//mysql methods
#include <mysql.h>
//cout / string
#include <iostream>
#include <string>
//vector
#include <vector>
//map
#include <map>

#include "db_object.hpp"

#ifndef __DATABASE_HPP__
# define __DATABASE_HPP__

# define DB_PORT 0
# define DB_UNIX_SOCKET NULL
# define DB_CLIENT_FLAG 0

class Database
{
public:
  ~Database();
  static Database* inst();
  DbObject* get_object(const std::string&, const std::string&, const std::string&) const;
  std::vector<DbObject*> get_objects(const std::string&, const std::string&, const std::string&) const;
  MYSQL_RES* do_query(const std::string&) const;
  void update(std::string*&);
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
