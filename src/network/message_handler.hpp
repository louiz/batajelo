/** @addtogroup Network
 *  @{
 */

/**
 * Handles the reading part of the socket.
 * Keeps a list of callbacks to call, associated with a message name, and
 * execute them when the associated message is received.
 * @class MessageHandler
 */

#ifndef MESSAGE_HANDLER_HPP
# define MESSAGE_HANDLER_HPP

#include <deque>
#include <map>
#include <functional>

#include <boost/asio.hpp>

#include <network/transfer_sender.hpp>
#include <network/base_socket.hpp>
#include <network/message.hpp>

typedef std::function<void(Message*)> t_read_callback;

class MessageHandler: public BaseSocket
{
public:
  explicit MessageHandler(boost::asio::io_service&);
  virtual ~MessageHandler();

  void install_read_handler();
  /**
   * called when there's something to read on the socket. Reads the message
   * the size of the arguments, and then calls binary_read_handler to read
   * the arguments of the message, if any.
   */
  void read_handler(const boost::system::error_code& error, const std::size_t bytes_transferred);
  /**
   * Read the arguments after a message (can read 0 bytes too) and pass that
   * to the callback that was associated with this message.
   */
  void binary_read_handler(const boost::system::error_code&, Message*, std::size_t, std::vector<t_read_callback> callbacks);
  /**
   * Sends a message, and use install_callback_once to wait for the answer
   * and call that callback to handle it.
   */
  void request_answer(Message*, t_read_callback on_answer, std::string name = "");
  /**
   * Install a new callback associated with a message. This callback will
   * be called upon receiving that message.
   */
  void install_callback(const std::string&, t_read_callback);
  /**
   * Install a new callback associated with a message. This callback will
   * be called upon receiving that message, but only once. This is used
   * for example if you send a message waiting for and answer, you install
   * a callback that will handle that answer, and only this one.
   */
  void install_callback_once(const std::string&, t_read_callback);
  /**
   * Remove a callback that has been installed.
   */
  void remove_callback(const std::string&);
  /**
   * Add the given message to the messages_to_send queue, then
   * calls check_messages_to_send. Which may send the next available
   * message, if there's no async_write() call already running.
   * It does not necessarily actually send the message on the socket.
   */
  void send(Message* message, std::function< void(void) > on_sent = 0);
  void send_message(const char* name, const google::protobuf::Message& msg);
  void send_message(const char* name, const std::string& archive);

protected:
  /**
   * Returns all the callbacks associated with the passed message name.
   * Returns an empty vector if nothing was found.
   */
  std::vector<t_read_callback> get_callbacks(const std::string&);
  /**
   * @todo Check if the data was correctly sent on the socket
   */
  void send_handler(const boost::system::error_code&, std::size_t, Message*);
  /**
   * Actually sends the message on the socket, calling async_write.
   */
  void actually_send(Message*);
  /**
   * Checks if there's something to send on the socket. We first check if
   * the writing boolean is false, and then we pop the next available message
   * from the queue (if any) and we send it using async_write.
   * @returns true if a message was sent, false otherwise.
   */
  bool check_messages_to_send();
  /**
   * A buffer keeping the data that is read on the socket.
   */
  boost::asio::streambuf data;
  /**
   * What happens when a read error occurs.
   */
  virtual void on_connection_closed() = 0;

private:
  MessageHandler(const MessageHandler&);
  MessageHandler& operator=(const MessageHandler&);

  std::multimap<const std::string, t_read_callback > callbacks;
  std::multimap<const std::string, t_read_callback > callbacks_once;
  /**
   * A queue of messages. If there's not async_write running, we pop one
   * from it and we send it.
   */
  std::deque<Message*> messages_to_send;
  /**
   * Tells us if we are waiting for an async_write to finish or not.
   * This must be set to true when calling async_write(), to false
   * in the write handler. It is used by check_messages_to_send.
   */
  bool writing;
};

#endif // MESSAGE_HANDLER_HPP
