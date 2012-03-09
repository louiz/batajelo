#include "client.hpp"

Client::Client(short port): port(port)
{
  this->socket = new tcp::socket(this->io_service);
}

Client::~Client()
{
  std::cout << "Closing connection" << std::endl;
  this->socket->close();
  delete this->socket;
}

// Connect, asyncly, and call one of the given callbacks
void Client::connect(boost::function< void(void) > on_success,
		     boost::function< void(void) > on_failure)
{
  // TODO use resolve and DNS
  tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), this->port);
  std::cout << "Connecting" << std::endl;
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
      std::cout << "Connection failed: " << error << std::endl;
      if (on_failure)
	on_failure();
    }
  else
    {
      std::cout << "Connected." << std::endl;
      if (on_success)
	on_success();
    }
}

void	Client::poll(void)
{
this->io_service.poll();
}

void	Client::send_message(void)
{
  std::cout << "sending coucou" << std::endl;
  this->send("coucou\n");
}

void	Client::send(const char* msg)
{
  async_write(*this->socket,
	      boost::asio::buffer(msg, strlen(msg)),
	      boost::bind(&Client::send_handler, this,
			  boost::asio::placeholders::error,
			  boost::asio::placeholders::bytes_transferred));
}

void	Client::send_handler(const boost::system::error_code& error,
			     std::size_t bytes_transferred)
{

}

int main (int ac, char **av)
{
  Client client(7878);
  client.connect(boost::bind(&Client::send_message, &client));
  client.poll();
}
