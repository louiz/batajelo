#include <network/remote_client.hpp>
#include <database/database.hpp>
#include <database/db_object.hpp>

RemoteClient::RemoteClient(boost::asio::io_service& io_service,
			   Server<RemoteClient>* server):
  InterfaceRemoteClient(io_service),
  server(server),
  user(0)
{
}

RemoteClient::~RemoteClient()
{
  log_info("Deleting remote client " << this->number);
  if (this->user != 0)
    delete this->user;
}

User* RemoteClient::get_user()
{
  return this->user;
}

void RemoteClient::install_callbacks()
{
  this->install_callback("AUTH", boost::bind(&RemoteClient::auth_callback, this, _1));
  this->install_callback("TRANSFER", boost::bind(&RemoteClient::transfer_callback, this, _1));
}

void RemoteClient::auth_callback(Command* received_command)
{
  Command* command = new Command();
  std::string arg(received_command->body, received_command->body_size);
  std::string name("AUTH");
  std::string body;
  bool success = false;
  log_debug("auth_callback: " << arg);
  size_t pos = arg.find('*');
  if (pos == std::string::npos)
    body = "1";
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
  	  body = "2";
  	}
      else if (user->get("password") != password)
  	{
  	  log_info("Authentication: Invalid password for user " << login);
  	  body = "3";
  	  delete user;
  	}
      // else if (this->server->find_client_by_login(login) != 0)
      // 	{
      // 	  log_info("Authentication: user already logged in from an other location: " << login);
      // 	  body = "4";
      // 	  delete user;
      // 	}
      else
  	{
  	  log_info("Authentication: succes for user " << login);
  	  body = "0";
  	  this->user = static_cast<User*>(user);
	  success = true;
  	}
    }
  command->set_name(name);
  command->set_body(body.c_str());
  this->send(command);
  if (success)
    this->on_auth_success();
}

void RemoteClient::transfer_callback(Command* received_command)
{
  this->send_file(std::string(received_command->body, received_command->body_size));
}

void RemoteClient::on_auth_success()
{
  log_debug("on_auth_success");
}

void RemoteClient::send_file(const std::string& filename)
{
  TransferSender* sender = new TransferSender(this, filename);
  if (sender->start() == true)
    {
      this->senders.push_back(sender);
      sender->send_next_chunk();
    }
  else
    delete sender;
}

void RemoteClient::on_connection_closed()
{
  this->server->remove_client(this);
}

boost::asio::io_service& RemoteClient::get_io_service()
{
  return this->server->io_service;
}

void RemoteClient::on_transfer_ended(const TransferSender* transfer)
{
  log_debug("on_transfer_ended");
  std::vector<TransferSender*>::iterator it;
  for (it = this->senders.begin(); it < this->senders.end(); ++it)
    if (*it == transfer)
      this->senders.erase(it);
  delete transfer;
}
