#ifndef DATABASE_HPP_INCLUDED
#define DATABASE_HPP_INCLUDED

#include <litesql.hpp>
#include <boost/optional.hpp>
#include <botan/botan.h>
#include <botan/bcrypt.h>
#include "batadb.hpp"

#include <tuple>
#include <string>

/**
 * Unique class that handles all database access.
 * It internally uses the litesql.
 */

class Database
{
public:
  Database();
  ~Database() = default;
  boost::optional<db::User> get_user_by_login(const std::string& login);
  std::tuple<bool, std::string> create_new_user(const std::string& login,
                                                const std::string& password);
  std::tuple<bool, std::string> check_user_password(const db::User& user,
                                                    const std::string& password) const;
  void change_user_password(db::User& user, const std::string& password);
  void set_user_password(db::User& user, const std::string& password);

  db::Game create_new_game() const;

private:
  Botan::AutoSeeded_RNG rng;
  db::BataDB db;
  Database(const Database&) = delete;
  Database(Database&&) = delete;
  Database& operator=(const Database&) = delete;
  Database& operator=(Database&&) = delete;
};

#endif /* DATABASE_HPP_INCLUDED */
