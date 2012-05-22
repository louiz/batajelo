#include <network/command_handler.hpp>
#include <network/command.hpp>

CommandHandler::CommandHandler():
  writing(false)
{
}

CommandHandler::~CommandHandler()
{
}

void CommandHandler::install_callback(const char name,
				      t_read_callback callback)
{
  log_debug("installing callback for command " << name);
  this->callbacks[name] = callback;
}

void CommandHandler::install_callback_once(const char name,
				      t_read_callback callback)
{
  log_debug("installing ONCE callback for command " << name);
  this->callbacks_once[name] = callback;
}

void CommandHandler::remove_callback(const char name)
{
  std::map<const char, t_read_callback >::iterator it;

  it = this->callbacks.find(name);
  if (it != this->callbacks.end())
    this->callbacks.erase(it);
  else
    log_warning("Could not remove callback: " << name);
}

t_read_callback CommandHandler::get_callback(const char name)
{
  std::map<const std::string, t_read_callback >::iterator it;

  it = this->callbacks.find(name);
  if (it != this->callbacks.end())
    return it->second;

  it = this->callbacks_once.find(name);
  if (it != this->callbacks_once.end())
    {
      log_debug("Removing callback for command " << name);
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
    {
      log_info("Read error: " << error);
      // TODO check more precisely for errors
      this->on_connection_closed();
      return;
    }
  if (bytes_transferred <= 1)
    {
      log_warning("read_handler: data received is too short to contain a command name and a size. Ignoring");
      return ;
    }
  // Extract the needed data from the buffer
  char *c = new char[bytes_transferred+1];
  this->data.sgetn(c, bytes_transferred);

  c[bytes_transferred] = 0;

  char command_name = c[0];
  // remove the ending : and the first char, to get the size argument.
  // If the data for the size is empty or invalid (not a number), atoi
  // returns 0, and that's how much we want to read in that case.
  std::size_t size = atoi(std::string(c+1, bytes_transferred-2).data());

  log_debug("Received command: " << command_name << size);
  Command* command = new Command;
  command->set_name(command_name);

  delete[] c;

  // Find out if a callback was registered for that command.
  t_read_callback callback = this->get_callback(command_name);
  // We check what we need to read on the socket to have the rest of the binary datas
  const std::size_t length_to_read = this->data.size() >= size ? 0 : size - this->data.size();

  boost::asio::async_read(*this->socket,
			  this->data,
			  boost::asio::transfer_at_least(length_to_read),
			  boost::bind(&CommandHandler::binary_read_handler, this,
				      boost::asio::placeholders::error,
				      command,
				      size, callback));
}

void CommandHandler::binary_read_handler(const boost::system::error_code& error,
					 Command* command,
					 std::size_t bytes_transferred,
					 t_read_callback callback)
{
  if (error)
    {
      log_error("binary_read_handler failed: "<< error);
      exit(1);
    }
  log_debug("binary_read_handler " << bytes_transferred);
  command->body = new char[bytes_transferred];
  this->data.sgetn(command->body, bytes_transferred);
  command->set_body_size(bytes_transferred);

  if (callback)
    callback(command);
  else
    log_debug("no callback");
  delete command;
  this->install_read_handler();
}

void CommandHandler::install_read_handler(void)
{
  boost::asio::async_read_until(*this->socket,
				this->data,
				':',
				boost::bind(&CommandHandler::read_handler, this,
					    boost::asio::placeholders::error,
					    boost::asio::placeholders::bytes_transferred));
}

void CommandHandler::request_answer(Command* command, t_read_callback on_answer, char name)
{
  if (name == '\0')
    name = command->get_name();
  this->install_callback_once(name, on_answer);
  this->send(command);
}

void CommandHandler::send(Command* command, boost::function< void(void) > on_sent)
{
  if (on_sent)
    command->callback = on_sent;
  log_debug("Sending command: " << command->get_name());
  this->commands_to_send.push_front(command);
  this->check_commands_to_send();
}

bool CommandHandler::check_commands_to_send()
{
  log_debug("Length of the queue: " << this->commands_to_send.size());
  if (this->writing || this->commands_to_send.empty())
    return false;
  this->actually_send(this->commands_to_send.back());
  this->commands_to_send.pop_back();
  return true;
}

void CommandHandler::actually_send(Command* command)
{
  this->writing = true;
  command->pack();
  std::vector<boost::asio::const_buffer> buffs;
  buffs.push_back(boost::asio::buffer(command->header.data(), command->header.length()));
  buffs.push_back(boost::asio::buffer(command->body, command->body_size));
  async_write(*this->socket,
	      buffs,
  	      boost::bind(&CommandHandler::send_handler, this,
  			  boost::asio::placeholders::error,
  			  boost::asio::placeholders::bytes_transferred,
  			  command));
}

void CommandHandler::send_handler(const boost::system::error_code& error,
				  std::size_t bytes_transferred,
				  Command* command)
{
  this->writing = false;
  // TODO, actually just disconnect in these cases.
  assert(bytes_transferred == command->header.length() + command->body_size);
  assert(!error);

  if (command->callback)
    command->callback();
  delete command;

  this->check_commands_to_send();
}
