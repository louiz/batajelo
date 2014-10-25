#include <logging/logging.hpp>
#include <network/client.hpp>
#include <boost/algorithm/string.hpp>

Client::Client()
{
}

Client::~Client()
{
}

void Client::install_callbacks()
{
  this->install_callback("TRANSFER", std::bind(&Client::transfer_init_callback, this, std::placeholders::_1));
}

void Client::transfer_init_callback(Message* received_message)
{
  std::string arg(received_message->body, received_message->body_size);
  std::vector<std::string> args;
  boost::split(args, arg, boost::is_any_of("|"));
  if (args.size() != 3)
    {
      log_warning("File transfer information are wrong: " << arg);
      return ;
    }
  log_debug("Starting file transfer reception: " << arg);
  TransferReceiver* receiver = new TransferReceiver(this, args[0], args[1], atoi(args[2].data()));
  this->receivers.push_back(receiver);
}

void Client::on_transfer_ended(const TransferReceiver* receiver)
{
  log_debug("on_transfer_ended");
  std::vector<TransferReceiver*>::iterator it;
  for (it = this->receivers.begin(); it < this->receivers.end(); ++it)
    if (*it == receiver)
      {
     this->receivers.erase(it);
     break ;
      }
  delete receiver;
}

void Client::on_connection_closed()
{

}
