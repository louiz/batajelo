#include <logging/logging.hpp>
#include <network/transfer_sender.hpp>
#include <network/remote_client.hpp>

#define CHUNK_SIZE 262144

unsigned long int TransferSender::current_id = 0;

TransferSender::TransferSender(RemoteClient* client, const std::string& filename):
  client(client),
  filename(filename)
{
  this->file.open(std::string(FILES_TO_SEND_DIRECTORY + this->filename).data(), std::ofstream::binary);

  std::ostringstream sid;
  sid << TransferSender::current_id++;
  this->id = std::string(sid.str());
}

TransferSender::~TransferSender()
{
  log_debug("Deleting file transfer of " << this->filename << ": " << this->id);
  this->file.close();
}

bool TransferSender::start()
{
  // log_debug("Starting file transfer " << this->id);
  if (this->file.good())
    {
      // get length of file:
      this->file.seekg(0, std::ios::end);
      this->length = this->file.tellg();
      this->file.seekg(0, std::ios::beg);
      std::ostringstream sid;
      sid << this->id;
      std::ostringstream slength;
      slength << this->length;
      std::string transfer_description(sid.str());
      transfer_description += "|" + this->filename + "|"  + slength.str();

      Command* command = new Command;
      command->set_name("TRANSFER");
      command->set_body(transfer_description.data(), transfer_description.size());
      this->client->send(command);
      return true;
    }
  log_warning("Could not open file to send: " << this->filename);
  return false;
}

void TransferSender::send_next_chunk()
{
  Command* command = new Command;

  command->body = new char[CHUNK_SIZE];
  std::streamsize read_size = this->file.readsome(command->body, CHUNK_SIZE);
  log_debug("Read from file: " << read_size);
  command->set_body_size(read_size);

  std::string command_name = "TRANSFER_" + this->id;
  command->set_name(command_name);
  if (read_size)
    {
      // this was not the last chunk, so we'll send an other one when this one is
      // successfully sent.
      this->client->send(command, std::bind(&TransferSender::send_next_chunk, this));
    }
  else
    this->client->send(command, std::bind(&RemoteClient::on_transfer_ended, this->client, this));
}
