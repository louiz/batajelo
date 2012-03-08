#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef __REMOTE_CLIENT_HPP__
# define __REMOTE_CLIENT_HPP__
using boost::asio::ip::tcp;

class RemoteClient
{
public:
  RemoteClient(boost::asio::io_service&);
  ~RemoteClient();
  void start(void);
  tcp::socket& get_socket(void);
  static int clients_number;

private:
  void read_handler(const boost::system::error_code&, std::size_t);
  void install_read_handler(void);

  boost::asio::streambuf data;
  tcp::socket socket;
  const int number;
};

#endif
