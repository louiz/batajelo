#include "remote_client.hpp"

int RemoteClient::clients_number = 0;

RemoteClient::RemoteClient(boost::asio::io_service& io_service):
  socket(io_service),
  number(RemoteClient::clients_number++)
{
}

tcp::socket& RemoteClient::get_socket(void)
{
  return socket;
}

void RemoteClient::read_handler(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  std::cout << "read_handler on client " << this->number << ": " << bytes_transferred << " bytes.   error: " << error << std::endl;

  if (error)
    {
      // TODO check more precisely for errors
      std::cout << "Stopping remote client " << this->number << " [" << error << "]. Exiting." << std::endl;
      return;
    }
  std::string s((std::istreambuf_iterator<char>(&this->data)),
		std::istreambuf_iterator<char>());
  std::cout << s << std::endl;
  // TODO, pass the string to a callback
  this->install_read_handler();
}

// Set a callback to be called whenever there’s something to read on the socket.
void RemoteClient::install_read_handler(void)
{
  boost::asio::async_read_until(this->socket,
			       this->data,
			       '\n',
			       boost::bind(&RemoteClient::read_handler, this,
					   boost::asio::placeholders::error,
					   boost::asio::placeholders::bytes_transferred));
}

void RemoteClient::start(void)
{
  std::cout << "Starting RemoteClient " << this->number << std::endl;
  this->install_read_handler();
}
