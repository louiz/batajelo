#include <logging/logging.hpp>
#include <network/interface_client.hpp>
#include <boost/algorithm/string.hpp>
#if defined(_WIN32) || defined(_WIN64)
# include <WinBase.h>
#endif

/**
 * Does nothing, it is just used to exit the io_service.run_one() after
 * a timeout.
 */
static void poll_timeout_handler(const boost::system::error_code&)
{
}

InterfaceClient::InterfaceClient():
  timeout(io_service)
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
                              std::function< void(void) > on_success,
                              std::function< void(void) > on_failure)
{
  // TODO use resolve and DNS
  tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
  this->socket->async_connect(endpoint,
                              std::bind(&InterfaceClient::connect_handler, this,
                                        on_success, on_failure,
                                        std::placeholders::_1));
  log_info("Connecting to " << host << ":" << port);
}

void InterfaceClient::connect_handler(std::function< void(void) > on_success,
			     std::function< void(void) > on_failure,
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
      this->install_callback("PING", std::bind(&InterfaceClient::ping_callback, this, std::placeholders::_1));
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
  if (timeout == 0)
    {
      this->io_service.poll();
      return ;
    }
  if (this->timeout.expires_from_now(boost::posix_time::milliseconds(timeout)) == 0)
    // The last run_one() call returned because the timeout expired, so
    // we reinstall it. If that's not the case
    // (something actually happened on the socket)
    // we just need to reset the time of expiration, but not reinstall it.
    this->timeout.async_wait(&poll_timeout_handler);
  // Wait for one event to happen (either a timeout or something
  // on the socket).
  this->io_service.run_one();
  while (this->io_service.poll() != 0)
    ; // Execute all other available handlers, if any.
}


