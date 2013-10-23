#include <logging/logging.hpp>
#include <network/command_handler.hpp>
#include <network/command.hpp>

CommandHandler::CommandHandler():
  writing(false)
{
}

CommandHandler::~CommandHandler()
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

void CommandHandler::remove_callback(const std::string& command)
{
  std::map<const std::string, t_read_callback >::iterator it;

  it = this->callbacks.find(command);
  if (it != this->callbacks.end())
    this->callbacks.erase(it);
  else
    log_warning("Could not remove callback: " << command);
}

t_read_callback CommandHandler::get_callback(const std::string& command)
{
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
    {
      log_debug("Read error: " << error);
      this->on_connection_closed();
      return;
    }
  if (bytes_transferred <= 1)
    {
      log_warning("Not enough data received for a command header to be valid.");
      return ;
    }

  // Extract the needed data from the buffer
  char *c = new char[bytes_transferred+1];
  this->data.sgetn(c, bytes_transferred);
  c[bytes_transferred] = 0;

  // find the . separator
  size_t pos = 0;
  while (c[pos] && c[pos] != '.')
    pos++;

  std::string command_name;
  std::size_t size;
  if (pos == bytes_transferred)
    {  // no '.' was found, the command name is assumed to 1 char long.
      command_name = std::string(1, c[0]);
      size = atoi(std::string(c+1, bytes_transferred-2).data());;
    }
  else
    {
      command_name = std::string(c, pos);
      size = atoi(std::string(c+pos+1, bytes_transferred-pos-2).data()); // remove the ending :
    }
  log_debug("Received : " << command_name << " . " << size);
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
                 std::bind(&CommandHandler::binary_read_handler, this,
                                    std::placeholders::_1,
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
                    std::bind(&CommandHandler::read_handler, this,
                                          std::placeholders::_1,
                                          std::placeholders::_2));
}

void CommandHandler::request_answer(Command* command, t_read_callback on_answer, std::string name)
{
  if (name.size() == 0)
    name = command->get_name();
  this->install_callback_once(name, on_answer);
  this->send(command);
}

void CommandHandler::send(Command* command, std::function< void(void) > on_sent)
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
             std::bind(&CommandHandler::send_handler, this,
                        std::placeholders::_1,
                        std::placeholders::_2,
                        command));
}

void CommandHandler::send_handler(const boost::system::error_code& error,
                      std::size_t bytes_transferred,
                      Command* command)
{
  this->writing = false;
  assert(bytes_transferred == command->header.length() + command->body_size);

  if (command->callback)
    command->callback();
  delete command;

  // TODO check for error
  if (error)
    exit(1);

  this->check_commands_to_send();
}
