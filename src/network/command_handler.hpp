/** @addtogroup Network
 *  @{
 */

/**
 * Handles the reading part of the socket.
 * Keeps a list of callbacks to call, associated with a command name, and
 * execute them when the associated command is received.
 * @class CommandHandler
 */

#include <deque>
#include <map>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef __COMMAND_HANDLER_HPP__
# define __COMMAND_HANDLER_HPP__

#include <network/transfer_sender.hpp>
#include <network/command.hpp>

using boost::asio::ip::tcp;

typedef boost::function<void(Command*)> t_read_callback;
typedef std::deque<Command*> command_queue;

class CommandHandler
{
  friend void TransferSender::send_next_chunk();
public:
  CommandHandler();
  virtual ~CommandHandler();

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
  void binary_read_handler(const boost::system::error_code&, Command*, std::size_t, t_read_callback);
  /**
   * Sends a command, and use install_callback_once to wait for the answer
   * and call that callback to handle it.
   */
  void request_answer(Command*, t_read_callback on_answer, std::string name = "");
  /**
   * Install a new callback associated with a command. This callback will
   * be called upon receiving that command.
   */
  void install_callback(const std::string&, t_read_callback);
  /**
   * Install a new callback associated with a command. This callback will
   * be called upon receiving that command, but only once. This is used
   * for example if you send a command waiting for and answer, you install
   * a callback that will handle that answer, and only this one.
   */
  void install_callback_once(const std::string&, t_read_callback);
  /**
   * Remove a callback that has been installed.
   */
  void remove_callback(const std::string&);
  /**
   * Add the given command to the commands_to_send queue, then
   * calls check_commands_to_send. Which may send the next available
   * command, if there's no async_write() call already running.
   * It does not necessarily actually send the command on the socket.
   */
  void send(Command* command, boost::function< void(void) > on_sent = 0);

protected:
  /**
   * Returns the callback associated with the passed command name.
   * Returns 0 if nothing was found, in that case the execution of the
   * return value cause a failure.
   */
  t_read_callback get_callback(const std::string&);
  /**
   * @todo Check if the data was correctly sent on the socket
   */
  void send_handler(const boost::system::error_code&, std::size_t, Command*);
  /**
   * Actually sends the command on the socket, calling async_write.
   */
  void actually_send(Command*);
  /**
   * Checks if there's something to send on the socket. We first check if
   * the writing boolean is false, and then we pop the next available command
   * from the queue (if any) and we send it using async_write.
   * @returns true if a command was sent, false otherwise.
   */
  bool check_commands_to_send();
  /**
   * A buffer keeping the data that is read on the socket.
   */
  boost::asio::streambuf data;
  /**
   * What happens when a read error occurs.
   */
  virtual void on_connection_closed() = 0;

  tcp::socket* socket;

private:
  CommandHandler(const CommandHandler&);
  CommandHandler& operator=(const CommandHandler&);

  std::map<const std::string, t_read_callback > callbacks;
  std::map<const std::string, t_read_callback > callbacks_once;
  /**
   * A queue of messages. If there's not async_write running, we pop one
   * from it and we send it.
   */
  command_queue commands_to_send;
  /**
   * Tells us if we are waiting for an async_write to finish or not.
   * This must be set to true when calling async_write(), to false
   * in the write handler. It is used by check_commands_to_send.
   */
  bool writing;
};

#endif // __COMMAND_HANDLER_HPP__
/**@}*/
