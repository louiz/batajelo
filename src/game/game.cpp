#include "game.hpp"
#include "../logging/logging.hpp"

#include <unistd.h>

Game::Game()
{
  log_info("Launching game");
}

Game::~Game()
{
  log_info("End.");
}

void Game::on_login_form_validated(const std::string& login,
				   const std::string& password,
				   const std::string& host,
				   const short& port)
{
  this->client.connect(host, port,
			boost::bind(&Game::on_connection_success, this, login, password),
			boost::bind(&Game::on_connection_failed, this, host, port));
}

void Game::on_connection_failed(const std::string& host, const short& port)
{
  log_debug("Connection to remote " << host << ":" << port << " failed");
}


void Game::on_connection_success(const std::string& login, const std::string& password)
{
  log_debug("Login in...");
  this->authenticate(login, password);
}

void Game::authenticate(const std::string& login, const std::string& password)
{
  this->client.request_answer("AUTH", login.data(), boost::bind(&Game::on_authenticate, this, _1));
}

void Game::on_authenticate(const std::string& result)
{
  log_debug("on_authenticate ");
}

void Game::run()
{
  while (true)
    {
      this->client.poll();
      usleep(10000);		// replace with game logic
    }
}

// Test main
// Simulate a login flow
int main(int argc, char** argv)
{
  Config::read_conf("../config/batajelo.conf");
  Game game;

  game.on_login_form_validated("test", "coucou", "127.0.0.1", 7878);
  game.run();
  return 0;
}
