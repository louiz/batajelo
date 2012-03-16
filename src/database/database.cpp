#include "database.hpp"
#include "../logging/logging.hpp"

Database* Database::instance = 0;

Database::Database()
{
  this->mysql = NULL;
}

Database::~Database() {}

Database* Database::inst()
{
  if (instance == 0)
		instance = new Database;
  return instance;
}

void Database::connect()
{
    this->mysql = mysql_init(NULL);
    if (this->mysql == NULL)
      log_error("Could'nt init a mysql connection.");
  if (mysql_real_connect(this->mysql, DB_HOST, DB_USER, DB_PASSWORD,
                          DB_DATABASE, DB_PORT, DB_UNIX_SOCKET, DB_CLIENT_FLAG) == NULL)
			log_error("Couldn't connect to the database.");
}

void Database::close()
{
  if (this->mysql != NULL)
    mysql_close(this->mysql);
}

DbObject* Database::get_object_by_id(const std::string& columns, const std::string& table, const std::string& where)
{
  MYSQL_RES* result = NULL;
  MYSQL_ROW mysql_row = NULL;
  MYSQL_FIELD* fields;
  unsigned int row_id = 0;
  unsigned int field_id = 0;
  unsigned int fields_number = 0;
  unsigned int error;
  DbObject* db_object = new DbObject;

	const std::string query = "SELECT " + columns + " FROM " + table + " WHERE " + where;
  this->connect();
  error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
		log_error("Couldn't query the database : " << error);
  result = mysql_use_result(this->mysql);
  if (result)
  {
    fields_number = mysql_num_fields(result);
    fields = mysql_fetch_fields(result);
    while ((mysql_row = mysql_fetch_row(result)))
    {
      for(; field_id < fields_number; field_id++)
        db_object->values.insert(std::make_pair(fields[field_id].name, mysql_row[field_id]));
    }
    mysql_free_result(result);
    this->close();
    return db_object;
  }
  else
   log_warning("No result found !\n");
}

std::vector<DbObject*> Database::get_objects_by_id(const std::string& columns, const std::string& table, const std::string& where)
{
	std::cout << "da_fu";
  MYSQL_RES* result = NULL;
  MYSQL_ROW mysql_row = NULL;
  MYSQL_FIELD* fields;
  unsigned int row_id = 0;
  unsigned int field_id = 0;
  unsigned int fields_number = 0;
  unsigned int error;
  std::cout << "0";
  DbObject* db_object = new DbObject;
  std::cout << "0.5";
  std::vector<DbObject*> db_objects;
	const std::string query = "SELECT " + columns + " FROM " + table + " WHERE " + where;

  this->connect();
  error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
		log_error("Couldn't query the database : " << error);
  result = mysql_use_result(this->mysql);
  if (result)
  {
    fields_number = mysql_num_fields(result);
    fields = mysql_fetch_fields(result);
    while ((mysql_row = mysql_fetch_row(result)))
    {
      for(field_id = 0; field_id < fields_number; field_id++)
      {
      	std::cout << "1";
        db_object->values.insert(std::make_pair(fields[field_id].name, mysql_row[field_id]));
        std::cout << "2";
				db_objects.push_back(db_object);
				db_object->values.clear();
      }
    }
    mysql_free_result(result);
    this->close();
    return db_objects;
  }
  else
		log_warning("No result found !\n");
}

void Database::update(std::string*& fields)
{
	unsigned int error;
	unsigned int i;
	std::map<std::string, std::string>::const_iterator it;
  std::string query = "UPDATE FROM ";
   //+ this->className();

	for (i = 0; i != sizeof(fields); i++)
	{
	/*	it = this->values.find(fields[i]);
		if (it != this->values.end())
			query += " SET " + fields[i] + " = " + it->second;
		else
			log_error("The field " << fields[i]() << " is not found");*/
	}
	query += " WHERE id = ";
	//+ id;

	this->connect();
	error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
		log_error("Couldn't query the database : " << error);

}
