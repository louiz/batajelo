#include <network/transfer_sender.hpp>
#include <network/remote_client.hpp>

#define CHUNK_SIZE 65536

unsigned long int TransferSender::current_id = 0;

TransferSender::TransferSender(RemoteClient* client, const std::string& filename):
  client(client),
  filename(filename),
  file(filename.data(), std::ofstream::binary)
{

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
  log_debug("Starting file transfer " << this->id);
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
      this->client->request_answer("TRANSFER", transfer_description.data());
      return true;
    }
  log_warning("Could not open file to send: " << this->filename);
  return false;
}

void TransferSender::send_next_chunk()
{
  char data[CHUNK_SIZE];

  std::streamsize read_size = this->file.readsome(data, CHUNK_SIZE);
  log_debug("Read from file: " << read_size);

  std::ostringstream ssize;
  ssize << read_size;

  std::string msg = "TRANSFER_" + this->id;
  msg += std::string(".") + std::string(ssize.str()) + std::string(":");
  this->client->send(msg.data());
  if (read_size)
    {
      // this was not the last chunk, so we'll send an other one when this one is
      // successfully sent.
      this->client->send(data, boost::bind(&TransferSender::send_next_chunk, this), read_size);
    }
  else
    {
      this->client->send(data, boost::bind(&RemoteClient::on_transfer_ended, this->client, this), read_size);
    }
}
