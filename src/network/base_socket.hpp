/**
 * A very simple base class that must be inherited by all network object
 * that needs a socket.
 * If an io_service is provided, we use it. Otherwise we use our own
 * io_service.
 */

#ifndef BASE_SOCKET
# define BASE_SOCKET

#include <boost/asio.hpp>

class BaseSocket
{
public:
  explicit BaseSocket(boost::asio::io_service& io_service):
    socket(io_service) {}
  const boost::asio::ip::tcp::socket& get_socket() const
  {
    return this->socket;
  }
  boost::asio::ip::tcp::socket& get_socket()
  {
    return this->socket;
  }
  ~BaseSocket() {}
protected:
  boost::asio::ip::tcp::socket socket;
private:
  BaseSocket(const BaseSocket&) = delete;
  BaseSocket(BaseSocket&&) = delete;
  BaseSocket& operator=(const BaseSocket&) = delete;
  BaseSocket& operator=(BaseSocket&&) = delete;
};

#endif // BASE_SOCKET

