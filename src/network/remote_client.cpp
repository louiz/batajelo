#include <network/remote_client.hpp>

RemoteClient::RemoteClient(boost::asio::io_service& io_service,
                           Server<RemoteClient>* server):
  RemoteClientBase(io_service),
  server(server)
{
}

RemoteClient::~RemoteClient()
{
  log_info("Deleting remote client " << this->id);
}

void RemoteClient::install_callbacks()
{
  this->install_callback("AUTH", std::bind(&RemoteClient::auth_callback, this, std::placeholders::_1));
  this->install_callback("TRANSFER", std::bind(&RemoteClient::transfer_callback, this, std::placeholders::_1));
}

void RemoteClient::auth_callback(Message*)
{
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
