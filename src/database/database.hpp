// Database Implementation, Objects of the tables...

#ifdef WIN32
//socket mysql_real_connect
# include <winsock.h>
#endif /* WIN32 */

//mysql methods
#include <mysql.h>
//cout
#include <iostream>
//stringstream
#include <sstream>
// exit
#include <stdio.h>
#include <stdlib.h>

#ifndef __DATABASE_HPP__
# define __DATABASE_HPP__

# define DB_HOST "localhost"
# define DB_USER "admin"
# define DB_PASSWORD "623ed0ede843b90fb79a862af71695d1"
# define DB_DATABASE "batajelo"
# define DB_PORT 0
# define DB_UNIX_SOCKET NULL
# define DB_CLIENT_FLAG 0

template <class Obj>
class Database
{
public:
  Database();
  ~Database();
  Obj queryById(const char*, const char*, const int);
private:
  MYSQL mysql;
  void connect();
  Obj query(const char*);
  void close();
};

#endif



