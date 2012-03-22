// The main class for the game itself

// Network
#include <network/client.hpp>

#ifndef __GAME_HPP__
# define __GAME_HPP__

class Game
{
public:
  Game();
  ~Game();
  // Called whenever the login form has been filled, clicked and checked for validity.
  void on_login_form_validated(const std::string&, const std::string&, const std::string&, const short&);
  void on_connection_failed(const std::string&, const short&);
  void on_connection_success(const std::string&, const std::string&);
  void authenticate(const std::string&, const std::string&);
  void on_authenticate(const std::string&);
  void run();

private:
  Client client;
  Game(const Game&);
  Game& operator=(const Game&);
};

#endif // __GAME_HPP__
