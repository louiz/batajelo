
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
  map_row row;
  MYSQL_RES* result = NULL;
  MYSQL_ROW mysql_row = NULL;
  MYSQL_FIELD* fields;
  unsigned int row_id = 0;
  unsigned int field_id = 0;
  unsigned int fields_number = 0;
  unsigned int error;

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
    while ((mysql_row = mysql_fetch_row(result)))
    {
      for(; field_id < fields_number; field_id++)
        row.insert(std::make_pair(fields[field_id].name, mysql_row[field_id]));
      this->results.push_back(std::make_pair(row_id++, row));
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


void Database::print()
{
  std::vector<std::pair<int, map_row> >::iterator it;
  map_row::iterator m;

  std::cout << "myvector contains:";
  for (it = this->results.begin() ; it != this->results.end(); it++ )
  {
      std::cout << it->first;
      map_row a = it->second;
      for (m = a.begin(); m != a.end(); m++)
      {
        std::cout << m->first << " -> " << m->second << std::endl;
      }
  }
}
