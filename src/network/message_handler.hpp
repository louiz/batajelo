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
#include <network/message.hpp>
#include <utils/scopeguard.hpp>

#include <logging/logging.hpp>

typedef std::function<void(Message*)> t_read_callback;

template <typename SocketType>
class MessageHandler: public SocketType
{
public:
  template <typename... SocketArgs>
  MessageHandler(SocketArgs&&... socket_args):
    SocketType(std::forward<SocketArgs>(socket_args)...),
    writing(false)
  { }
  virtual ~MessageHandler() = default;

  void install_read_handler()
  {
    boost::asio::async_read_until(this->get_stream(),
                                  this->data,
                                  ':',
                                  std::bind(&MessageHandler::read_handler, this,
                                            std::placeholders::_1,
                                            std::placeholders::_2));
  }

  /**
   * called when there's something to read on the socket. Reads the message
   * the size of the arguments, and then calls binary_read_handler to read
   * the arguments of the message, if any.
   */
  void read_handler(const boost::system::error_code& error, const std::size_t bytes_transferred)
  {
    log_debug("read_handler, size: " << bytes_transferred << " bytes.");
    if (error)
      {
        log_debug("Read error: " << error);
        this->on_connection_closed();
        return;
      }
    if (bytes_transferred <= 1)
      {
        log_warning("Not enough data received for a message header to be valid.");
        return ;
      }

    char *c = new char[bytes_transferred+1];

    // Make sure c will ALWAYS be deleted
    utils::ScopeGuard guard([c](){ delete[] c;});

    // Extract the needed data from the buffer
    this->data.sgetn(c, bytes_transferred);
    c[bytes_transferred] = 0;

    // find the . separator
    std::size_t pos = 0;
    while (c[pos] && c[pos] != '.')
      pos++;

    std::string message_name;
    std::size_t size;
    if (pos == bytes_transferred)
      {  // no '.' was found, the message name is assumed to 1 char long.
        message_name = std::string(1, c[0]);
        size = atoi(std::string(c+1, bytes_transferred-2).data());;
      }
    else
      {
        message_name = std::string(c, pos);
        size = atoi(std::string(c+pos+1, bytes_transferred-pos-2).data()); // remove the ending :
      }
    log_debug("Received : " << message_name << " . " << size);
    Message* message = new Message;
    message->set_name(message_name);


    // Find out if a callback was registered for that message.
    auto callbacks = this->get_callbacks(message_name);
    // We check what we need to read on the socket to have the rest of the binary datas
    const std::size_t length_to_read = this->data.size() >= size ? 0 : size - this->data.size();

    boost::asio::async_read(this->get_stream(),
                            this->data,
                            boost::asio::transfer_at_least(length_to_read),
                            std::bind(&MessageHandler::binary_read_handler, this,
                                      std::placeholders::_1,
                                      message,
                                      size, callbacks));
  }

  /**
   * Read the arguments after a message (can read 0 bytes too) and pass that
   * to the callback that was associated with this message.
   */
  void binary_read_handler(const boost::system::error_code& error,
                           Message* message, std::size_t bytes_transferred,
                           std::vector<t_read_callback> callbacks)
  {
    if (error)
      {
        log_error("binary_read_handler failed: "<< error);
        exit(1);
      }
    log_debug("binary_read_handler " << bytes_transferred);
    message->body = new char[bytes_transferred];
    this->data.sgetn(message->body, bytes_transferred);
    message->set_body_size(bytes_transferred);

    for (const auto& cb: callbacks)
      {
        try {
          cb(message);
        }
        catch (const SerializationException& error)
          {
            log_error("Invalid message received.");
          }
      }
    delete message;
    this->install_read_handler();
  }

  /**
   * Sends a message, and use install_callback_once to wait for the answer
   * and call that callback to handle it.
   */
  void request_answer(Message* message, t_read_callback on_answer, std::string name="")
  {
    if (name.size() == 0)
      name = message->get_name();
    this->install_callback_once(name, on_answer);
    this->send(message);
  }


  /**
   * Install a new callback associated with a message. This callback will
   * be called upon receiving that message.
   */
  void install_callback(const std::string& name, t_read_callback callback)
  {
    log_debug("installing callback for message " << name);
    this->callbacks.emplace(name, callback);
  }

  /**
   * Install a new callback associated with a message. This callback will
   * be called upon receiving that message, but only once. This is used
   * for example if you send a message waiting for and answer, you install
   * a callback that will handle that answer, and only this one.
   */
  void install_callback_once(const std::string& name, t_read_callback callback)
  {
    log_debug("installing ONCE callback for message " << name);
    this->callbacks_once.emplace(name, callback);
  }

  /**
   * Remove a callback that has been installed.
   */
  void remove_callback(const std::string& name)
  {
    log_debug("remove_callback: " << name);
    auto res = this->callbacks.erase(name);
    log_warning("Removed " << res << " callbacks");
  }

  /**
   * Add the given message to the messages_to_send queue, then
   * calls check_messages_to_send. Which may send the next available
   * message, if there's no async_write() call already running.
   * It does not necessarily actually send the message on the socket.
   */
  void send(Message* message, std::function<void(void)> on_sent=nullptr)
  {
    if (on_sent)
      message->callback = on_sent;
    log_debug("Sending message: " << message->get_name());
    this->messages_to_send.push_front(message);
    this->check_messages_to_send();
  }

  void send_message(const char* name, const google::protobuf::Message& msg)
  {
    Message* message = new Message;
    message->set_name(name);
    message->set_body(msg);
    this->send(message);
  }

  void send_message(const char* name, const std::string& archive)
  {
    Message* message = new Message;
    message->set_name(name);
    message->set_body(archive.data(), archive.length());
    this->send(message);
  }


protected:
  /**
   * Returns all the callbacks associated with the passed message name.
   * Returns an empty vector if nothing was found.
   */
  std::vector<t_read_callback> get_callbacks(const std::string& message)
  {
    std::vector<t_read_callback> res;

    auto its = this->callbacks.equal_range(message);
    for (auto it = std::get<0>(its); it != std::get<1>(its); ++it)
      res.push_back(it->second);

    its = this->callbacks_once.equal_range(message);
    for (auto it = std::get<0>(its); it != std::get<1>(its); ++it)
      res.push_back(it->second);
    this->callbacks_once.erase(std::get<0>(its), std::get<1>(its));
    return res;
  }

  /**
   * @todo Check if the data was correctly sent on the socket
   */
  void send_handler(const boost::system::error_code& error, std::size_t bytes_transferred, Message* message)
  {
    this->writing = false;

    // TODO check for error
    if (error)
      exit(1);

    assert(bytes_transferred == message->header.length() + message->body_size);

    if (message->callback)
      message->callback();
    delete message;


    this->check_messages_to_send();
  }


  /**
   * Actually sends the message on the socket, calling async_write.
   */
  void actually_send(Message* message)
  {
    this->writing = true;
    message->pack();
    std::vector<boost::asio::const_buffer> buffs;
    buffs.push_back(boost::asio::buffer(message->header.data(), message->header.length()));
    buffs.push_back(boost::asio::buffer(message->body, message->body_size));
    async_write(this->get_stream(),
                buffs,
                std::bind(&MessageHandler::send_handler, this,
                          std::placeholders::_1,
                          std::placeholders::_2,
                          message));
  }

  /**
   * Checks if there's something to send on the socket. We first check if
   * the writing boolean is false, and then we pop the next available message
   * from the queue (if any) and we send it using async_write.
   * @returns true if a message was sent, false otherwise.
   */
  bool check_messages_to_send()
  {
    if (this->writing || this->messages_to_send.empty())
      return false;
    this->actually_send(this->messages_to_send.back());
    this->messages_to_send.pop_back();
    return true;
  }

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
