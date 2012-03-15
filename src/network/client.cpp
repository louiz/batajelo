#include "client.hpp"

Client::Client()
{
  this->socket = new tcp::socket(io_service);
}

Client::~Client()
{
  if (this->socket->is_open())
    {
      log_debug << "Closing connection" << std::endl;
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
  log_info << "Connecting" << std::endl;
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
      log_info << "Connection failed: " << error << std::endl;
      if (on_failure)
	on_failure();
    }
  else
    {
      log_info << "Connected." << std::endl;
      this->install_read_handler();
      if (on_success)
	on_success();
    }
}

void	Client::poll(void)
{
  this->io_service.poll();
}

