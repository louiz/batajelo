#include <network/command_handler.hpp>

CommandHandler::CommandHandler()
{
}

void CommandHandler::install_callback(const std::string& command,
				      t_read_callback callback)
{
  log_debug("installing callback for command " << command);
  this->callbacks[command] = callback;
}

void CommandHandler::install_callback_once(const std::string& command,
				      t_read_callback callback)
{
  log_debug("installing ONCE callback for command " << command);
  this->callbacks_once[command] = callback;
}

t_read_callback CommandHandler::get_callback(const std::string& command)
{
  log_debug("get_callback");
  std::map<const std::string, t_read_callback >::iterator it;

  it = this->callbacks.find(command);
  if (it != this->callbacks.end())
    return it->second;

  it = this->callbacks_once.find(command);
  if (it != this->callbacks_once.end())
    {
      log_debug("Removing callback for command " << command);
      t_read_callback callback = it->second;
      this->callbacks_once.erase(it);
      return callback;
    }
  return 0;
}

void CommandHandler::read_handler(const boost::system::error_code& error, const std::size_t bytes_transferred)
{
  log_debug("read_handler, size: " << bytes_transferred << " bytes.");
  if (error)
    log_debug("Read error: " << error);
  if (error)
    {
      // TODO check more precisely for errors
      this->on_connection_closed();
      return;
    }
  // Extract the needed data from the buffer
  char *c = new char[bytes_transferred+1];
  this->data.sgetn(c, bytes_transferred);

  c[bytes_transferred] = 0;
  // find the . separator
  size_t pos = 0;
  while (c[pos] && c[pos] != '.')
    pos++;

  std::string command;
  std::size_t size;
  if (pos == bytes_transferred)
    {  // no . was found
      command = std::string(c, pos-1);
      size = 0;
    }
  else
    {
      command = std::string(c, pos);
      size = atoi(std::string(c+pos+1, bytes_transferred-pos-2).data()); // remove the ending :
    }
  log_debug(command << " . " << size);
  delete[] c;

  // Find out if a callback was registered for that command.
  t_read_callback callback = this->get_callback(command);
  // We check what we need to read on the socket to have the rest of the binary datas
  const std::size_t length_to_read = this->data.size() >= size ? 0 : size - this->data.size();

  log_debug("length to read on the socket: " << length_to_read);
  boost::asio::async_read(*this->socket,
			  this->data,
			  boost::asio::transfer_at_least(length_to_read),
			  boost::bind(&CommandHandler::binary_read_handler, this,
				      boost::asio::placeholders::error,
				      size, callback));
}

void CommandHandler::binary_read_handler(const boost::system::error_code& error,
					 std::size_t bytes_transferred,
					 // std::size_t size,
					 t_read_callback callback)
{
  log_debug("binary_read_handler " << bytes_transferred);
  char *c = new char[bytes_transferred+1];
  this->data.sgetn(c, bytes_transferred);

  c[bytes_transferred] = 0;
  if (callback)
    callback(c, bytes_transferred);
  else
    log_debug("no callback");
  delete[] c;
  this->install_read_handler();
}

// Set a callback to be called whenever there’s something to read on the socket.
void CommandHandler::install_read_handler(void)
{
  boost::asio::async_read_until(*this->socket,
				this->data,
				':',
				boost::bind(&CommandHandler::read_handler, this,
					    boost::asio::placeholders::error,
					    boost::asio::placeholders::bytes_transferred));
}

// Send a command and add a callback to be called when the answer to
// this command will be received
void CommandHandler::request_answer(const char* command, const char* data,
				    t_read_callback on_answer)
{
  std::string msg(command);

  std::ostringstream ssize;
  ssize << strlen(data);
  msg += std::string(".") + std::string(ssize.str()) + std::string(":") + std::string(data);

  // We may want to send a command that do not require an answer.
  if (on_answer)
    this->install_callback_once(command, on_answer);
  this->send(msg.data());
}

void CommandHandler::send(const char* msg, boost::function< void(void) > on_sent, int length)
{
  if (length == 0)
    length = strlen(msg);
  async_write(*this->socket,
	      boost::asio::buffer(msg, length),
	      boost::bind(&CommandHandler::send_handler, this,
			  boost::asio::placeholders::error,
			  boost::asio::placeholders::bytes_transferred,
			  on_sent));
  log_debug("Sending [" << length << "] bytes");
}

void CommandHandler::send_handler(const boost::system::error_code& error,
				  std::size_t bytes_transferred,
				  boost::function< void(void) > on_sent)
{
  // TODO check for error
  if (error)
    exit(1);
  log_debug(bytes_transferred << " bytes sent");
  if (on_sent)
    on_sent();
}
