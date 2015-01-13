#include <logging/logging.hpp>
#include <network/client_base.hpp>
#include <boost/algorithm/string.hpp>

ClientBase::ClientBase():
  BaseIoservice(),
  MessageHandler(io_service)
{
}

ClientBase::~ClientBase()
{
  if (this->socket.is_open())
    {
      log_debug("Closing connection");
      this->socket.close();
    }
}

// Connect, asyncly, and call one of the given callbacks
void ClientBase::connect(const std::string& host,
                         const short& port,
                         std::function<void(void)> on_success,
                         std::function<void(void)> on_failure)
{
  // TODO use resolve and DNS
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
  this->socket.async_connect(endpoint,
                              std::bind(&ClientBase::connect_handler, this,
                                        on_success, on_failure,
                                        std::placeholders::_1));
  log_info("Connecting to " << host << ":" << port);
}

void ClientBase::connect_handler(std::function< void(void) > on_success,
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
      this->install_callback("PING", std::bind(&ClientBase::ping_callback, this, std::placeholders::_1));
      this->install_callbacks();
      this->install_read_handler();
      if (on_success)
        on_success();
    }
}

void ClientBase::ping_callback(Message*)
{
  log_debug("Received PING");

  Message* message = new Message;
  message->set_name("PONG");
  this->send(message);
}

void ClientBase::poll()
{
  while (this->io_service.poll() != 0)
    ;
}


