#include <master_server/remote_client.hpp>
#include <master_server/master_server.hpp>
#include "master.pb.h"

RemoteClient::RemoteClient():
  user(nullptr),
  server(nullptr),
  senders{}
{
}

RemoteClient::~RemoteClient()
{
  log_info("Deleting remote client " << this->id);
}

void RemoteClient::set_server(MasterServer* server)
{
  this->server = server;
}

const db::User* RemoteClient::get_user() const
{
  return this->user.get();
}

void RemoteClient::install_callbacks()
{
  this->install_callback("AUTH", std::bind(&RemoteClient::auth_callback, this, std::placeholders::_1));
  this->install_callback("TRANSFER", std::bind(&RemoteClient::transfer_callback, this, std::placeholders::_1));
}

void RemoteClient::auth_callback(Message* message)
{
  auto srl = message->parse_body_to_protobuf_object<ser::master::Authenticate>();
  ser::master::AuthResponse response;
  auto db = this->server->get_database();
  auto maybe_user = db->get_user_by_login(srl.login());
  if (!maybe_user)
    response.set_result(ser::master::AuthResponse::ERROR_INVALID_LOGIN);
  else
    {
      auto user = *maybe_user;
      auto res = db->check_user_password(user, srl.password());
      if (std::get<0>(res) == true)
        {
          this->user = std::make_unique<db::User>(user);
          response.set_result(ser::master::AuthResponse::SUCCESS);
        }
      else
        response.set_result(ser::master::AuthResponse::ERROR_INVALID_PASSWORD);
    }
  this->send_message("AUTH_RESPONSE", response);
  if (this->user)
    this->on_user_logged_in();
}


void RemoteClient::on_user_logged_in()
{
  // TODO send various information, the friend list, the news, etc
}

void RemoteClient::transfer_callback(Message* received_message)
{
  this->send_file(std::string(received_message->body, received_message->body_size));
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

void RemoteClient::on_transfer_ended(const TransferSender* transfer)
{
  log_debug("on_transfer_ended");
  std::vector<TransferSender*>::iterator it;
  for (it = this->senders.begin(); it < this->senders.end(); ++it)
    if (*it == transfer)
      {
     this->senders.erase(it);
     break ;
      }
  delete transfer;
}
