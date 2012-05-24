#include <game/game.hpp>
#include <logging/logging.hpp>

Game::Game()
{
  this->ui = new Ui(this);
  log_info("Launching game");
}

Game::~Game()
{
  delete this->ui;
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
//  this->ui->on_connection();
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
  this->client.request_answer(command, boost::bind(&Ui::on_authenticate, this->ui, _1));
}

void Game::request_file(const std::string& filename)
{
  Command* command = new Command;
  command->set_name("TRANSFER");
  command->set_body(filename.data(), filename.size());
  this->client.send(command);
}

void Game::run()
{
  while (this->ui->run() == true)
  {
    this->client.poll();
    this->ui->handleEvent();
    this->ui->refresh();
  }
}
