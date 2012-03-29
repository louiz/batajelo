/** @addtogroup Network
 *  @{
 */

/**
 * Handles the reading part of the socket.
 * Keeps a list of callbacks to call, associated with a command name, and
 * execute them when the associated command is received.
 * @class CommandHandler
 */

#include <map>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <logging/logging.hpp>

#ifndef __COMMAND_HANDLER_HPP__
# define __COMMAND_HANDLER_HPP__

#include <network/transfer_sender.hpp>
using boost::asio::ip::tcp;

class CommandHandler
{
  friend void TransferSender::send_next_chunk();
public:
  CommandHandler();
  ~CommandHandler() {}

  void install_read_handler();
  /**
   * called when there's something to read on the socket. Reads the command
   * the size of the arguments, and then calls binary_read_handler to read
   * the arguments of the commad, if any.
   */
  void read_handler(const boost::system::error_code& error, const std::size_t bytes_transferred);
  /**
   * Read the arguments after a command (can read 0 bytes too) and pass that
   * to the callback that was associated with this command.
   */
  void binary_read_handler(const boost::system::error_code&, std::size_t, boost::function<void(std::string)>);
  /**
   * Sends a command, and use install_callback_once to wait for the answer
   * and call that callback to handle it.
   */
  void request_answer(const char*, const char*, boost::function< void(const std::string&) > on_answer = 0);

protected:
  /**
   * Install a new callback associated with a command. This callback will
   * be called upon receiving that command.
   */
  void install_callback(const std::string&, boost::function< void(std::string) >);
  /**
   * Install a new callback associated with a command. This callback will
   * be called upon receiving that command, but only once. This is used
   * for example if you send a command waiting for and answer, you install
   * a callback that will handle that answer, and only this one.
   */
  void install_callback_once(const std::string&, boost::function< void(std::string) >);
  /**
   * Returns the callback associated with the passed command name.
   * Returns 0 if nothing was found, in that case the execution of the
   * return value cause a failure.
   */
  boost::function< void(std::string) > get_callback(const std::string&);
  /**
   * Send the given data on the socket.
   */
  void send(const char*, boost::function< void(void) > on_send = 0);
  /**
   * @todo Check if the data was correctly sent on the socket
   */
  void send_handler(const boost::system::error_code&, std::size_t, boost::function< void(void) > on_sent);
  /**
   * A buffer keeping the data that is read on the socket.
   */
  /**
   * What happens when a read error occurs.
   */
  virtual void on_connection_closed() = 0;

  boost::asio::streambuf data;
  tcp::socket* socket;

private:
  CommandHandler(const CommandHandler&);
  CommandHandler& operator=(const CommandHandler&);

  std::map<const std::string, boost::function< void(std::string) > > callbacks;
  std::map<const std::string, boost::function< void(std::string) > > callbacks_once;
};

#endif // __COMMAND_HANDLER_HPP__
/**@}*/
