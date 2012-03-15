#include <map>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "../logging/logging.hpp"

#ifndef __COMMAND_HANDLER_HPP__
# define __COMMAND_HANDLER_HPP__

using boost::asio::ip::tcp;

// Handles the reading part of the socket
// Keeps a list of callbacks to call, associated with a command name
class CommandHandler
{
public:
  CommandHandler();
  ~CommandHandler() {}
  void read_handler(const boost::system::error_code& error, std::size_t bytes_transferred);
  void install_read_handler();

  void binary_read_handler(const boost::system::error_code&, std::size_t, boost::function<void(std::string)>);
  void request_answer(const char*, const char*, boost::function< void(const std::string&) >);

protected:
  tcp::socket* socket;
  void install_callback(const std::string&, boost::function< void(std::string) >);
  void install_callback_once(const std::string&, boost::function< void(std::string) >);
  boost::function< void(std::string) > get_callback(const std::string&);
  boost::asio::streambuf data;
  void send(const char*);
  void send_handler(const boost::system::error_code&, std::size_t);

private:
  CommandHandler(const CommandHandler&);
  CommandHandler& operator=(const CommandHandler&);

  std::map<const std::string, boost::function< void(std::string) > > callbacks;
  std::map<const std::string, boost::function< void(std::string) > > callbacks_once;
};

#endif // __COMMAND_HANDLER_HPP__
