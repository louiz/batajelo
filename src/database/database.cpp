#include "database.hpp"
#include "../logging/logging.hpp"
#include "../config/config.hpp"

Database* Database::instance = 0;

Database::Database()
{
  this->mysql = NULL;
  this->connect();
}

Database::~Database()
{
  this->close();
}

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
	log_debug("Connecting to database using: host:" << Config::get("db_host", "localhost").data() << " user:" << Config::get("db_user", "root").data() << " pass:" << Config::get("db_password", "").data() << " db:" << Config::get("db_database", "batajelo").data());
	if (mysql_real_connect(this->mysql,
			 Config::get("db_host", "localhost").data(), Config::get("db_user", "root").data(),
			 Config::get("db_password", "").data(), Config::get("db_database", "batajelo").data(),
			 DB_PORT, DB_UNIX_SOCKET, DB_CLIENT_FLAG) == NULL)
	{
		log_error("Couldn't connect to the database.");
	}
	else
	{
		log_debug("Connected to database");
	}
}

void Database::close()
{
  if (this->mysql != NULL)
    mysql_close(this->mysql);
}

MYSQL_RES* Database::do_query(const std::string& query) const
{

  log_debug("Doing query [" << query << "]");
  const unsigned int error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
    {
      log_error("Couldn't query the database: " << error);
      return NULL;
    }
  MYSQL_RES* result = mysql_use_result(this->mysql);
  if (!result)
    {
      log_warning("No result found!");
      return NULL;
    }
  return result;
}

DbObject* Database::get_object(const std::string& columns,
			       const std::string& table,
			       const std::string& where) const
{
  MYSQL_RES* result = this->do_query("SELECT " + columns + " FROM " + table + " WHERE " + where);
  if (!result)
    return NULL;

  MYSQL_ROW mysql_row = mysql_fetch_row(result);
  // If there’s no result, we just return NULL
  if (mysql_row == NULL )
    {
      log_warning("No row returned");
      return NULL;
    }
  // We can fill the object with the data from the database
  DbObject* db_object = new DbObject;
  unsigned int fields_number = mysql_num_fields(result);
  unsigned int field_id = 0;
  MYSQL_FIELD* fields = mysql_fetch_fields(result);
  for (; field_id < fields_number; field_id++)
    if (mysql_row[field_id])  // if the value is NULL, we don’t append anything
      db_object->values.insert(std::make_pair(fields[field_id].name, mysql_row[field_id]));
#ifdef DEBUG
  // If there are more than one result, we might want to use get_objects instead
  if (mysql_fetch_row(result) != NULL)
    log_warning("More than one row returned in get_object.");
#endif
  mysql_free_result(result);

  return db_object;
}

std::vector<DbObject*> Database::get_objects(const std::string& columns,
					     const std::string& table,
					     const std::string& where) const
{
  MYSQL_RES* result = this->do_query("SELECT " + columns + " FROM " + table + " WHERE " + where);
  std::vector<DbObject*> db_objects;

  // It's OK to return an empty list. We must always check that the vector
  // is not empty when using this method.
  if (!result)
    return db_objects;

  unsigned int field_id = 0;
  unsigned int fields_number = mysql_num_fields(result);
  MYSQL_FIELD* fields = mysql_fetch_fields(result);
  MYSQL_ROW mysql_row = NULL;
  while ((mysql_row = mysql_fetch_row(result)))
    {
      log_debug("Inserting one object in result vector");
      DbObject* db_object = new DbObject;
      for(field_id = 0; field_id < fields_number; field_id++)
	if (mysql_row[field_id])  // if the value is NULL, we don’t append anything
	  db_object->values.insert(std::make_pair(fields[field_id].name, mysql_row[field_id]));
      db_objects.push_back(db_object);
    }
  mysql_free_result(result);
  return db_objects;
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

	error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
		log_error("Couldn't query the database : " << error);

}

 void Database::insert()
 {
 }
