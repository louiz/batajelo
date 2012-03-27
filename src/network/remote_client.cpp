#include <network/remote_client.hpp>
#include <network/server.hpp>
#include <database/database.hpp>
#include <database/db_object.hpp>

unsigned long int RemoteClient::clients_number = 0;

RemoteClient::RemoteClient(boost::asio::io_service& io_service,
			   Server* server):
  number(RemoteClient::clients_number++),
  server(0),
  user(0)
{
  this->socket = new tcp::socket(io_service);
  this->server = server;
}

RemoteClient::~RemoteClient()
{
  log_info("Deleting remote client " << this->number);
  delete this->socket;
  delete this->user;
}

tcp::socket& RemoteClient::get_socket(void)
{
  return *this->socket;
}

User* RemoteClient::get_user()
{
  return this->user;
}

void RemoteClient::install_callbacks()
{
  this->install_callback("AUTH", boost::bind(&RemoteClient::auth_callback, this, _1));
}

void RemoteClient::auth_callback(const std::string& arg)
{
  std::string res("AUTH.");

  log_debug("auth_callback: " << arg);
  size_t pos = arg.find('*');
  if (pos == std::string::npos)
    res += "1:1";
  else
    {
      std::string login = arg.substr(0, pos);
      std::string password = arg.substr(pos+1);
      log_debug(login << ":" << password);
      DbObject* user = Database::inst()->get_object("*", "User",
						    std::string("`login`='" + login + "'"));
      if (user == 0)
  	{
  	  log_info("Authentication: User " << login << " does not exist in database.");
  	  res += "1:2";
  	}
      else if (user->get("password") != password)
  	{
  	  log_info("Authentication: Invalid password for user " << login);
  	  res += "1:3";
  	  delete user;
  	}
      else if (this->server->find_client_by_login(login) != 0)
  	{
  	  log_info("Authentication: user already logged in from an other location: " << login);
  	  res += "1:4";
  	  delete user;
  	}
      else
  	{
  	  log_info("Authentication: succes for user " << login);
  	  res += "1:0";
  	  this->user = static_cast<User*>(user);
  	}
    }
  this->send(res.data());
}

void RemoteClient::start()
{
  log_debug("Starting RemoteClient " << this->number);
  CommandHandler::install_read_handler();
  this->install_callbacks();
}

void RemoteClient::on_connection_closed()
{
  this->server->remove_client(this);
}
