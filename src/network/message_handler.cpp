#include <logging/logging.hpp>
#include <network/message_handler.hpp>
#include <network/message.hpp>

MessageHandler::MessageHandler(boost::asio::io_service& io_service):
  BaseSocket(io_service),
  writing(false)
{
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::install_callback(const std::string& message,
                                      t_read_callback callback)
{
  log_debug("installing callback for message " << message);
  this->callbacks.emplace(message, callback);
}

void MessageHandler::install_callback_once(const std::string& message,
                                           t_read_callback callback)
{
  log_debug("installing ONCE callback for message " << message);
  this->callbacks_once.emplace(message, callback);
}

void MessageHandler::remove_callback(const std::string& message)
{
  log_debug("remove_callback: " << message);
  auto res = this->callbacks.erase(message);
  log_warning("Removed " << res << " callbacks");
}

std::vector<t_read_callback> MessageHandler::get_callbacks(const std::string& message)
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

void MessageHandler::read_handler(const boost::system::error_code& error, const std::size_t bytes_transferred)
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

  // Extract the needed data from the buffer
  char *c = new char[bytes_transferred+1];
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

  delete[] c;

  // Find out if a callback was registered for that message.
  auto callbacks = this->get_callbacks(message_name);
  // We check what we need to read on the socket to have the rest of the binary datas
  const std::size_t length_to_read = this->data.size() >= size ? 0 : size - this->data.size();

  boost::asio::async_read(this->socket,
                          this->data,
                          boost::asio::transfer_at_least(length_to_read),
                          std::bind(&MessageHandler::binary_read_handler, this,
                                    std::placeholders::_1,
                                    message,
                                    size, callbacks));
}

void MessageHandler::binary_read_handler(const boost::system::error_code& error,
                                         Message* message,
                                         std::size_t bytes_transferred,
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

  if (callbacks.empty())
    {
      log_debug("no callback");
    }
  else
    {
      for (const auto& cb: callbacks)
        cb(message);
    }
  delete message;
  this->install_read_handler();
}

void MessageHandler::install_read_handler(void)
{
  boost::asio::async_read_until(this->socket,
                    this->data,
                    ':',
                    std::bind(&MessageHandler::read_handler, this,
                                          std::placeholders::_1,
                                          std::placeholders::_2));
}

void MessageHandler::request_answer(Message* message, t_read_callback on_answer, std::string name)
{
  if (name.size() == 0)
    name = message->get_name();
  this->install_callback_once(name, on_answer);
  this->send(message);
}

void MessageHandler::send(Message* message, std::function< void(void) > on_sent)
{
  if (on_sent)
    message->callback = on_sent;
  log_debug("Sending message: " << message->get_name());
  this->messages_to_send.push_front(message);
  this->check_messages_to_send();
}

bool MessageHandler::check_messages_to_send()
{
  log_debug("Length of the queue: " << this->messages_to_send.size());
  if (this->writing || this->messages_to_send.empty())
    return false;
  this->actually_send(this->messages_to_send.back());
  this->messages_to_send.pop_back();
  return true;
}

void MessageHandler::actually_send(Message* message)
{
  this->writing = true;
  message->pack();
  std::vector<boost::asio::const_buffer> buffs;
  buffs.push_back(boost::asio::buffer(message->header.data(), message->header.length()));
  buffs.push_back(boost::asio::buffer(message->body, message->body_size));
  async_write(this->socket,
              buffs,
              std::bind(&MessageHandler::send_handler, this,
                        std::placeholders::_1,
                        std::placeholders::_2,
                        message));
}

void MessageHandler::send_handler(const boost::system::error_code& error,
                      std::size_t bytes_transferred,
                      Message* message)
{
  this->writing = false;
  assert(bytes_transferred == message->header.length() + message->body_size);

  if (message->callback)
    message->callback();
  delete message;

  // TODO check for error
  if (error)
    exit(1);

  this->check_messages_to_send();
}
