#include <network/transfer_receiver.hpp>
#include <network/client.hpp>

TransferReceiver::TransferReceiver(Client* client,
				   const std::string& sid,
				   const std::string& filename,
				   int length):
  client(client),
  id(sid),
  filename(filename),
  length(length),
  received_length(0)
{
  std::string command_name("TRANSFER_");
  command_name += sid;
  this->client->install_callback_once(command_name, boost::bind(&TransferReceiver::get_next_chunk, this, _1, _2));
  this->file.open(this->filename.data(), std::ofstream::binary);
}

TransferReceiver::~TransferReceiver()
{
  log_debug("Deleting transfer of " << this->filename << ": " << this->id);
  this->file.close();
}

void TransferReceiver::get_next_chunk(const char* data, int length)
{
  log_debug("coucou");
  if (!this->file.good())
    {
      log_warning("Could not write data to file " << this->filename << ". Stopping transfer");
      return ;
    }
  if (length == 0)
    {
      // file transfer is over.
      this->client->on_transfer_ended(this);
    }
  else
    {
      log_debug("Writing " << length << " bytes to file");
      this->file.write(data, length);

      std::ostringstream sid;
      sid << this->id;
      std::string command_name("TRANSFER_");
      command_name += sid.str();
      // transfer is not over yet, add the callback again to get the next chunk.
      this->client->install_callback_once(command_name, boost::bind(&TransferReceiver::get_next_chunk, this, _1, _2));
    }
}
