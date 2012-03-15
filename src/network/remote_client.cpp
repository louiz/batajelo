#include "remote_client.hpp"

int RemoteClient::clients_number = 0;

RemoteClient::RemoteClient(boost::asio::io_service& io_service):
  number(RemoteClient::clients_number++)
{
  this->socket = new tcp::socket(io_service);
}

RemoteClient::~RemoteClient()
{
  log_info << "Deleting remote client" << std::endl;
  delete this->socket;
}

tcp::socket& RemoteClient::get_socket(void)
{
  return *this->socket;
}

void RemoteClient::install_callbacks()
{
  this->install_callback("AUTH", boost::bind(&RemoteClient::auth_callback, this, _1));
}

void RemoteClient::auth_callback(const std::string& arg)
{
  log_debug << "auth_callback: " << arg << std::endl;
  // TODO check login for user, check if it is already logged,
  // set it as logged in the database, and return 0 if it failed and 1
  // if it succeded
  this->send("AUTH.1:1");
}

void RemoteClient::start(void)
{
  log_debug << "Starting RemoteClient " << this->number << std::endl;
  CommandHandler::install_read_handler();
  this->install_callbacks();
}
