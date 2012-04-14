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
  Command* command = new Command;
  command->set_name("AUTH");
  std::string body = login + '*' + password;
  command->set_body(body.data(), body.size());
  this->client.request_answer(command, boost::bind(&Game::on_authenticate, this, _1));
}

void Game::request_file(const std::string& filename)
{
  Command* command = new Command;
  command->set_name("TRANSFER");
  command->set_body(filename.data(), filename.size());
  this->client.send(command);
}

void Game::on_authenticate(Command* received_command)
{
  std::string result(received_command->body, received_command->body_size);
  int res = atoi(result.data());
  log_debug("on_authenticate :" << res << "." <<  ((res > 4) ? "Unknown error" : auth_messages[res]));
  if (res == 0)
    this->request_file("file.bin");
}

void Game::run()
{
  while (true)
    {
      this->client.poll();
      usleep(10000);		// replace with game logic
    }
}
