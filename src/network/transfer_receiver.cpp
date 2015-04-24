#include <logging/logging.hpp>
#include <network/transfer_receiver.hpp>
#include <network/message_handler.hpp>
#include <network/tls_socket.hpp>

using namespace std::string_literals;

static const boost::filesystem::path FILES_TO_RECEIVE_DIRECTORY("./receive/");

TransferReceiver::TransferReceiver(MessageHandler<TLSSocket>* client,
                                   const std::string& sid,
                                   const std::string& filename,
                                   int length,
                                   std::function<void(const TransferReceiver*)>&& end_callback):
  client(client),
  id(sid),
  filename(FILES_TO_RECEIVE_DIRECTORY / filename),
  length(length),
  received_length(0),
  end_callback(end_callback)
{
  this->client->install_callback("TRANSFER_"s + sid, std::bind(&TransferReceiver::get_next_chunk, this, std::placeholders::_1));
  this->file.open(this->filename, std::ofstream::binary);
}

TransferReceiver::~TransferReceiver()
{
  log_debug("Deleting transfer of " << this->filename << ": " << this->id);
  this->file.close();
}

void TransferReceiver::get_next_chunk(Message* received_message)
{
  if (!this->file.good())
    {
      log_warning("Could not write data to file " << this->filename << ". Stopping transfer");
      this->stop();
      return ;
    }
  if (received_message->body_size == 0)
    this->stop();
  else
    this->file.write(received_message->body, received_message->body_size);
}

void TransferReceiver::stop()
{
  this->client->remove_callback("TRANSFER_"s + this->id);
  this->end_callback(this);
}
