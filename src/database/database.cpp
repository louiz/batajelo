

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

Obj Database::query(const char* query)
{
  Type x;
  MYSQL_RES* result = NULL;
  MYSQL_ROW row = NULL;
  unsigned int i = 0;
  unsigned int num_fields = 0;
  unsigned int error;
  error = mysql_query(&this->mysql, query);
  if (error != 0)
  {
    printf("Couldn't query the database : %d\n", error);
  }
  result = mysql_use_result(&this->mysql);
  if (result)
  {
    num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result)))
    {
      for(i = 0; i < num_fields; i++)
      {
        printf("[%s] ",  row[i]);
      }
    }
    mysql_free_result(result);
  }
  else
  {
    printf("No result found !\n");
  }
  return x;
}

void Database::close()
{
  if (&this->mysql != NULL)
    mysql_close(&this->mysql);
}

Obj queryById(const char *table, const char *fields, const int id)
{
  Type x;
  std::stringstream ss;
  std::string query;
  std::string* res;

  this->connect();
  ss << "SELECT " << fields << " FROM "
  << table << " WHERE id = " << id;
  query = ss.str();
  x = this->query(query.c_str());
  this->close();
  return x;
}
