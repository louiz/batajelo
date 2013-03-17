#include <gui/network_to_ui.hpp>
#include <logging/logging.hpp>

NetworkToUi::NetworkToUi()
{
  this->ui = new Ui(this);
  log_info("Launching game");
}

NetworkToUi::~NetworkToUi()
{
  delete this->ui;
  log_info("End.");
}

void NetworkToUi::on_login_form_validated(const std::string& login,
                                   const std::string& password,
                                   const std::string& host,
                                   const short& port)
{
  this->client.connect(host, port,
           std::bind(&NetworkToUi::on_connection_success, this, login, password),
           std::bind(&NetworkToUi::on_connection_failed, this, host, port));
}

void NetworkToUi::on_connection_failed(const std::string& host, const short& port)
{
  log_debug("Connection to remote " << host << ":" << port << " failed");
  this->ui->on_connection_failed();
}


void NetworkToUi::on_connection_success(const std::string& login, const std::string& password)
{
  log_debug("Login in...")
  this->ui->on_connection_success();
  this->authenticate(login, password);
}

void NetworkToUi::authenticate(const std::string& login, const std::string& password)
{
  // TODO send a password hash, and do not use that stupid separator.
  Command* command = new Command;
  command->set_name("AUTH");
  std::string body = login + '*' + password;
  log_error(body);
  command->set_body(body.data(), body.size());
  this->client.request_answer(command, std::bind(&Ui::on_authenticate, this->ui, std::placeholders::_1));
}

void NetworkToUi::request_file(const std::string& filename)
{
  Command* command = new Command;
  command->set_name("TRANSFER");
  command->set_body(filename.data(), filename.size());
  this->client.send(command);
}

void NetworkToUi::run()
{
  while (this->ui->run() == true)
  {
    this->client.poll();
    this->ui->handle_event();
    this->ui->refresh();
  }
}
