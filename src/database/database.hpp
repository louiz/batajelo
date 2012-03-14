// Database Implementation

#ifdef WIN32
//socket mysql_real_connect
# include <winsock.h>
#endif /* WIN32 */

//mysql methods
#include <mysql.h>
//cout
#include <iostream>
//vector
#include <vector>
//map
#include <map>

#include "db_object.hpp"

#ifndef __DATABASE_HPP__
# define __DATABASE_HPP__

# define DB_HOST "localhost"
# define DB_USER "admin"
# define DB_PASSWORD "623ed0ede843b90fb79a862af71695d1"
# define DB_DATABASE "batajelo"
# define DB_PORT 0
# define DB_UNIX_SOCKET NULL
# define DB_CLIENT_FLAG 0

class Database
{
public:
  Database();
  ~Database();
  DbObject get_object_by_id(const std::string&, const std::string&);
  std::vector<DbObject> get_objects_by_id(const std::string&, const std::string&);
  void update(std::map<std::string, std::string>);
  void insert();
private:
  void connect();
  DbObject query_object(std::string const&, bool);
  void close();

  MYSQL *mysql;
};
#endif
