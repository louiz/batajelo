#ifndef DATABASE_HPP_INCLUDED
#define DATABASE_HPP_INCLUDED

#include "batadb.hpp"

/**
 * Unique class that handles all database access.
 * It internally uses the litesql.
 */

class Database
{
public:
  Database();
  ~Database() = default;

private:
  Database(const Database&) = delete;
  Database(Database&&) = delete;
  Database& operator=(const Database&) = delete;
  Database& operator=(Database&&) = delete;
};

#endif /* DATABASE_HPP_INCLUDED */
