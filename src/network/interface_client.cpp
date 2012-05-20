#include <network/interface_client.hpp>
#include <boost/algorithm/string.hpp>

InterfaceClient::InterfaceClient()
{
  this->socket = new tcp::socket(io_service);
}

InterfaceClient::~InterfaceClient()
{
  if (this->socket->is_open())
    {
      log_debug("Closing connection");
      this->socket->close();
    }
  delete this->socket;
}

// Connect, asyncly, and call one of the given callbacks
void InterfaceClient::connect(const std::string& host,
		     const short& port,
		     boost::function< void(void) > on_success,
		     boost::function< void(void) > on_failure)
{
  // TODO use resolve and DNS
  tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
  log_info("Connecting");
  this->socket->async_connect(endpoint,
			     boost::bind(&InterfaceClient::connect_handler, this,
					 on_success, on_failure,
					 boost::asio::placeholders::error));
}

void InterfaceClient::connect_handler(boost::function< void(void) > on_success,
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
      this->install_callback("PING", boost::bind(&InterfaceClient::ping_callback, this, _1));
      this->install_callbacks();
      this->install_read_handler();
      if (on_success)
	on_success();
      
    }
}

void InterfaceClient::ping_callback(Command*)
{
  log_debug("Received PING");

  Command* command = new Command;
  command->set_name("PONG");
  this->send(command);
}

boost::asio::io_service& InterfaceClient::get_io_service()
{
  return this->io_service;
}

void InterfaceClient::poll(long timeout)
{
  this->io_service.poll();
  if (timeout == 0)
    return ;
  for (timeout *= 2; timeout > 0; timeout--)
    {
      usleep(500);
      this->io_service.poll();
    }
}

