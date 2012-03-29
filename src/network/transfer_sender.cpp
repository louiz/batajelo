#include <network/transfer_sender.hpp>
#include <network/remote_client.hpp>

unsigned long int TransferSender::current_id = 0;

TransferSender::TransferSender(RemoteClient* client, const std::string& filename):
  client(client),
  filename(filename),
  file(filename.data()),
  id(TransferSender::current_id++)
{
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
  char data[4096+1];

  this->file.read(data, 4096);
  std::streamsize read_size = this->file.gcount();
  log_debug("Read from file: " << read_size);
  data[read_size] = 0;

  std::ostringstream sid;
  sid << this->id;

  std::ostringstream ssize;
  ssize << read_size;

  std::string msg = "TRANSFER_" + std::string(sid.str());
  msg += std::string(".") + std::string(ssize.str()) + std::string(":") + std::string(data);
  if (read_size)
    // this was not the last chunk, so we'll send an other one when this one is
    // successfully sent.
    this->client->send(msg.data(), boost::bind(&TransferSender::send_next_chunk, this));
  else
    this->client->send(msg.data(), boost::bind(&RemoteClient::on_transfer_ended, this->client, this));
}
