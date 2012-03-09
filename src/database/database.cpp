
#include "database.hpp"

Database::Database() {}

Database::~Database() {}

void Database::connect()
{
  if (mysql_init(&mysql) == NULL)
  {
    std::cout << "Could'nt init a mysql connection." << std::endl;
  }

  if (!mysql_real_connect(&this->mysql, DB_HOST, DB_USER, DB_PASSWORD,
                          DB_DATABASE, DB_PORT, DB_UNIX_SOCKET, DB_CLIENT_FLAG))
  {
    std::cout << "Couldn't connect to the database." << std::endl;
    exit -1;
  }

}

void Database::query(std::string& query)
{
  MYSQL_RES* result = NULL;
  MYSQL_ROW row = NULL;
  MYSQL_FIELD* fields;
  unsigned int i = 0;
  unsigned int fields_number = 0;
  unsigned int error;
  std::map<std::string, std::string> results;

  error = mysql_query(&this->mysql, query.c_str());
  if (error != 0)
  {
    printf("Couldn't query the database : %d\n", error);
  }
  result = mysql_use_result(&this->mysql);
  if (result)
  {
    fields_number = mysql_num_fields(result);
    fields = mysql_fetch_fields(result);
    while ((row = mysql_fetch_row(result)))
    {
      for(; i < fields_number; i++)
      {
        fields = mysql_fetch_fields(result);
        printf("[%s : %s] ",  fields[i].name, row[i]);
      }
    }
    mysql_free_result(result);
  }
  else
  {
    printf("No result found !\n");
  }
}

void Database::close()
{
  if (&this->mysql != NULL)
    mysql_close(&this->mysql);
}

void Database::query_by_id(std::string table, std::string fields, std::string join, std::string id)
{
  std::string query;

  this->connect();
  query = "SELECT " + fields + " FROM " + table + " " + join + " WHERE id = " + id;
  this->query(query);
  this->close();
}
