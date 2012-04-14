#include <network/client.hpp>
#include <boost/algorithm/string.hpp>

Client::Client()
{
  this->socket = new tcp::socket(io_service);
}

Client::~Client()
{
  if (this->socket->is_open())
    {
      log_debug("Closing connection");
      this->socket->close();
    }
  delete this->socket;
}

// Connect, asyncly, and call one of the given callbacks
void Client::connect(const std::string& host,
		     const short& port,
		     boost::function< void(void) > on_success,
		     boost::function< void(void) > on_failure)
{
  // TODO use resolve and DNS
  tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
  log_info("Connecting");
  this->socket->async_connect(endpoint,
			     boost::bind(&Client::connect_handler, this,
					 on_success, on_failure,
					 boost::asio::placeholders::error));
}

void Client::connect_handler(boost::function< void(void) > on_success,
			     boost::function< void(void) > on_failure,
			     const boost::system::error_code& error)
{
  if (error)
    {
      log_info("Connection failed: " << error);
      if (on_failure)
	on_failure();
    }
  else
    {
      log_info("Connected.");
      this->install_read_handler();
      this->install_callbacks();
      if (on_success)
	on_success();
    }
}

void Client::install_callbacks()
{
  this->install_callback("TRANSFER", boost::bind(&Client::transfer_init_callback, this, _1));
}

void Client::transfer_init_callback(Command* received_command)
{
  std::string arg(received_command->body, received_command->body_size);
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
      this->receivers.erase(it);
  delete receiver;
  exit(0);
}

void Client::poll(void)
{
  this->io_service.poll();
}

