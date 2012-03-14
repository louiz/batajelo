#include "database.hpp"

Database::Database()
{
  this->mysql = NULL;
}

Database::~Database() {}

void Database::connect()
{
  if (this->mysql == NULL)
  {
    this->mysql = mysql_init(NULL);
    if (this->mysql == NULL)
      std::cout << "Could'nt init a mysql connection." << std::endl;
  }
  if (!mysql_real_connect(this->mysql, DB_HOST, DB_USER, DB_PASSWORD,
                          DB_DATABASE, DB_PORT, DB_UNIX_SOCKET, DB_CLIENT_FLAG))
    std::cout << "Couldn't connect to the database." << std::endl;
    exit (-1);
  }
}

void Database::close()
{
  if (this->mysql != NULL)
    mysql_close(this->mysql);
}

DbObject Database::get_object_by_id(const std::string& table, const std::string& id)
{
  MYSQL_RES* result = NULL;
  MYSQL_ROW mysql_row = NULL;
  MYSQL_FIELD* fields;
  unsigned int row_id = 0;
  unsigned int field_id = 0;
  unsigned int fields_number = 0;
  unsigned int error;
  DbObject db_object;

  const std::string query = "SELECT * FROM " + table + " WHERE id = " + id;
  this->connect();
  error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
  {
    printf("Couldn't query the database : %d\n", error);
  }
  result = mysql_use_result(this->mysql);
  if (result)
  {
    fields_number = mysql_num_fields(result);
    fields = mysql_fetch_fields(result);
    while ((mysql_row = mysql_fetch_row(result)))
    {
      for(; field_id < fields_number; field_id++)
        db_object.values.insert(std::make_pair(fields[field_id].name, mysql_row[field_id]));
    }
    mysql_free_result(result);
    this->close();
    return db_object;
  }
  else
    printf("No result found !\n");
}

std::vector<DbObject> Database::get_objects_by_id(const std::string& table, const std::string& id)
{
  MYSQL_RES* result = NULL;
  MYSQL_ROW mysql_row = NULL;
  MYSQL_FIELD* fields;
  unsigned int row_id = 0;
  unsigned int field_id = 0;
  unsigned int fields_number = 0;
  unsigned int error;
  DbObject db_object;
  std::vector<DbObject> db_objects;

  const std::string query = "SELECT * FROM " + table + " WHERE id = " + id;
  this->connect();
  error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
  {
    printf("Couldn't query the database : %d\n", error);
  }
  result = mysql_use_result(this->mysql);
  if (result)
  {
    fields_number = mysql_num_fields(result);
    fields = mysql_fetch_fields(result);
    while ((mysql_row = mysql_fetch_row(result)))
    {
      for(; field_id < fields_number; field_id++)
        db_object.values.insert(std::make_pair(fields[field_id].name, mysql_row[field_id]));
      db_objects.push_back(db_object);
    }
    mysql_free_result(result);
    this->close();
    return db_objects;
  }
  else
    printf("No result found !\n");
}
