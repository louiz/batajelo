#include <database/database.hpp>
#include <logging/logging.hpp>
#include <config/config.hpp>

Database* Database::instance = 0;

Database::Database():
  mysql(0),
  connected(false)
{
  if (this->init() == 0)
    exit(1);
}

Database::~Database()
{
  this->close();
}

bool Database::init()
{
  this->mysql = mysql_init(NULL);
  if (this->mysql == NULL)
    {
      log_error("Could'nt init a mysql connection.");
      return false;
    }
  return true;
}

Database* Database::inst()
{
  if (instance == 0)
    instance = new Database;
  return instance;
}

bool Database::connect()
{
  if (this->connected == true)
    return true;
  log_debug("Connecting to database using: host:" << Config::get("db_host", "localhost").data() << " user:" << Config::get("db_user", "root").data() << " pass:" << Config::get("db_password", "").data() << " db:" << Config::get("db_database", "batajelo").data());
  if (mysql_real_connect(this->mysql,
			 Config::get("db_host", "localhost").data(),
			 Config::get("db_user", "root").data(),
			 Config::get("db_password", "").data(),
			 Config::get("db_database", "batajelo").data(),
			 DB_PORT, DB_UNIX_SOCKET, DB_CLIENT_FLAG) == NULL)
    {
      log_error("Could'nt connect to the database.");
      return false;
    }
  else
    {
      log_debug("Connected to database");
      this->connected = true;
      return true;
    }
}

void Database::close()
{
  if (this->mysql != NULL)
    mysql_close(this->mysql);
  this->connected = false;
}

MYSQL_RES* Database::do_query(const std::string& query)
{
  if (this->connect() == false)
    return NULL;
  log_debug("Doing query [" << query << "]");
  const unsigned int error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
    {
      log_error("Couldn't query the database: " << error);
      this->connected = false;
      return NULL;
    }
  MYSQL_RES* result = mysql_use_result(this->mysql);
  if (!result)
    return NULL;
  return result;
}

bool Database::do_update(const std::string& query)
{
  if (!this->connect())
    return false;
  log_debug("Doing update [" << query << "]");
  const unsigned int error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
    {
      log_error("Couldn't query the database: " << error);
      this->connected = false;
      return false;
    }
  if (mysql_affected_rows(this->mysql) == 0)
    return false;
  return true;
}

bool Database::do_remove(const std::string& query)
{
  if (!this->connect())
    return false;
  log_debug("Doing delete [" << query << "]");
  const unsigned int error = mysql_query(this->mysql, query.c_str());
  if (error != 0)
    {
      log_error("Couldn't query the database: " << error);
      this->connected = false;
      return false;
    }
  if (mysql_affected_rows(this->mysql) == 0)
    return false;
  return true;
}

DbObject* Database::get_object(const std::string& columns,
			       const std::string& table,
			       const std::string& where)
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
					     const std::string& where)
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
      DbObject* db_object = new DbObject;
      for(field_id = 0; field_id < fields_number; field_id++)
	if (mysql_row[field_id])  // if the value is NULL, we don’t append anything
	  db_object->values.insert(std::make_pair(fields[field_id].name, mysql_row[field_id]));
      db_objects.push_back(db_object);
    }
  mysql_free_result(result);
  return db_objects;
}

const bool Database::update(const DbObject* object, const std::string& table_name)
{
  std::string query = "INSERT INTO " + table_name;
  std::string fields_str = "(";
  std::string values_str = "VALUES (";
  std::string update_str = "ON DUPLICATE KEY UPDATE ";

  std::map<std::string, std::string>::const_iterator it;
  // Use this iterator to detect the last element in the vector
  std::map<std::string, std::string>::const_iterator final_it = object->values.end();
  --final_it;

  // unsigned int i;
  for (it = object->values.begin(); it != object->values.end(); ++it)
    {
      fields_str += "`" + it->first + "`";
      values_str += "'" + it->second + "'";
      update_str += "`" + it->first + "`='" + it->second + "'";
      if (it != final_it)
	{
	  fields_str += ",";
	  values_str += ",";
	  update_str += ",";
	}
    }
  query += fields_str + ") " + values_str + ") " + update_str;

  if (this->do_update(query) == false)
    return false;
  return true;
};

const bool Database::remove(const std::string& table_name, const std::string& where)
{
  std::string query = "DELETE FROM " + table_name;
  query += " WHERE " + where;

  if (this->do_remove(query) == false)
    return false;
  return true;
};
