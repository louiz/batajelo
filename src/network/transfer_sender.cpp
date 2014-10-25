#include <iostream>
#include <logging/logging.hpp>
#include <network/transfer_sender.hpp>
#include <network/remote_client.hpp>

using namespace std::string_literals;

static constexpr std::size_t CHUNK_SIZE = 262144u;

unsigned long int TransferSender::current_id = 0;

TransferSender::TransferSender(RemoteClient* client, const std::string& filename):
  client(client),
  filename(filename)
{
  fs::path file_name = FILES_TO_SEND_DIRECTORY / this->filename;
  this->file.open(file_name, std::ofstream::binary);

  this->id = std::to_string(TransferSender::current_id++);
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

      Message* message = new Message;
      message->set_name("TRANSFER");
      message->set_body(transfer_description.data(), transfer_description.size());
      this->client->send(message);
      return true;
    }
  log_warning("Could not open file to send: " << this->filename);
  return false;
}

void TransferSender::send_next_chunk()
{
  Message* message = new Message;

  message->body = new char[CHUNK_SIZE];
  std::streamsize read_size = this->file.readsome(message->body, CHUNK_SIZE);
  log_debug("Read from file: " << read_size);
  message->set_body_size(read_size);

  std::string message_name = "TRANSFER_" + this->id;
  message->set_name(message_name);
  if (read_size)
    {
      // this was not the last chunk, so we'll send an other one when this one is
      // successfully sent.
      this->client->send(message, std::bind(&TransferSender::send_next_chunk, this));
    }
  else
    this->client->send(message, std::bind(&RemoteClient::on_transfer_ended, this->client, this));
}
