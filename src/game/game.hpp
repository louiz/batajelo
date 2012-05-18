// The main class for the game itself


#ifndef __GAME_HPP__
# define __GAME_HPP__

#include <network/client.hpp>
#include <network/command.hpp>
#include <gui/ui.hpp>

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
  void on_authenticate(Command*);
  /**
   * Sends a request for a file transfer.
   * @param filename The file that we want to receive.
   */
  void request_file(const std::string&);

  void run();

private:
  Client client;
	Ui ui;
  Game(const Game&);
  Game& operator=(const Game&);
};

#endif // __GAME_HPP__
