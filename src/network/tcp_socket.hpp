/**
 * A very simple base class that must be inherited by all network object
 * that needs a simple TCP socket
 */

#ifndef TCP_SOCKET
# define TCP_SOCKET

#include <boost/asio.hpp>
#include <network/ioservice.hpp>

class TCPSocket
{
public:
  explicit TCPSocket():
    socket(IoService::get())
  {
  }
  ~TCPSocket() = default;
  const boost::asio::ip::tcp::socket& get_socket() const
  {
    return this->socket;
  }
  boost::asio::ip::tcp::socket& get_socket()
  {
    return this->socket;
  }

private:
  boost::asio::ip::tcp::socket socket;

  TCPSocket(const TCPSocket&) = delete;
  TCPSocket(TCPSocket&&) = delete;
  TCPSocket& operator=(const TCPSocket&) = delete;
  TCPSocket& operator=(TCPSocket&&) = delete;
};

#endif // TCP_SOCKET
