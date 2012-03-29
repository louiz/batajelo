#include <game/game.hpp>
#include <logging/logging.hpp>

#include <unistd.h>

static const char* auth_messages[] = {"Success",
				      "Unknown error",
				      "User does not exist",
				      "Invalid password",
                                      "User already logged from an other location"};
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
  // TODO send a password hash, and do not use that stupid separator.
  this->client.request_answer("AUTH", std::string(login + '*' + password).data(), boost::bind(&Game::on_authenticate, this, _1));
}

void Game::request_file(const std::string& filename)
{
  this->client.request_answer("TRANSFER", filename.data());
}

void Game::on_authenticate(const std::string& result)
{
  int res = atoi(result.data());
  log_debug("on_authenticate :" << res << "." <<  ((res > 4) ? "Unknown error" : auth_messages[res]));
  if (res == 0)
    this->request_file("file.txt");
  // TODO some UI stuf, etc.
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
  Config::read_conf("../../batajelo.conf");
  Game game;

  game.on_login_form_validated("testing", "new_pass", "127.0.0.1", 7878);
  game.run();
  return 0;
}
