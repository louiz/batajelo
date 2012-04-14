#include <network/transfer_receiver.hpp>
#include <network/client.hpp>

TransferReceiver::TransferReceiver(Client* client,
				   const std::string& sid,
				   const std::string& filename,
				   int length):
  client(client),
  id(sid),
  filename(FILES_TO_RECEIVE_DIRECTORY + filename),
  length(length),
  received_length(0)
{
  std::string command_name("TRANSFER_");
  command_name += sid;
  this->client->install_callback(command_name, boost::bind(&TransferReceiver::get_next_chunk, this, _1));
  this->file.open(this->filename.data(), std::ofstream::binary);
}

TransferReceiver::~TransferReceiver()
{
  log_debug("Deleting transfer of " << this->filename << ": " << this->id);
  this->file.close();
}

void TransferReceiver::get_next_chunk(Command* received_command)
{
  if (!this->file.good())
    {
      log_warning("Could not write data to file " << this->filename << ". Stopping transfer");
      this->stop();
      return ;
    }
  if (received_command->body_size == 0)
    this->stop();
  else
    this->file.write(received_command->body, received_command->body_size);
}

void TransferReceiver::stop()
{
  std::ostringstream sid;
  sid << this->id;
  std::string command_name("TRANSFER_");
  command_name += sid.str();
  this->client->remove_callback(command_name);
  this->client->on_transfer_ended(this);
}
