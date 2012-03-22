#include <network/command_handler.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef __REMOTE_CLIENT_HPP__
# define __REMOTE_CLIENT_HPP__

using boost::asio::ip::tcp;

class RemoteClient: public CommandHandler
{
public:
  RemoteClient(boost::asio::io_service&);
  ~RemoteClient();
  void start(void);
  tcp::socket& get_socket(void);

  static int clients_number;

private:
  void install_callbacks();
  void install_read_handler(void);
  void auth_callback(const std::string&);

  const int number;
};

#endif
