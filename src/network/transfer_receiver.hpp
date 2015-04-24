/** @addtogroup Network
 *  @{
 */

/**
 * Represent an outgoing file transfer.
 * @class TransferReceiver
 */

#ifndef TRANSFER_RECEIVER_HPP
# define TRANSFER_RECEIVER_HPP

#include <iostream>
#include <string>
#include <functional>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <network/message.hpp>

class TLSSocket;

template <typename SocketType>
class MessageHandler;

class TransferReceiver
{
public:
  TransferReceiver(MessageHandler<TLSSocket>*, const std::string&, const std::string&, int,
                   std::function<void(const TransferReceiver*)>&&);
  ~TransferReceiver();
  void stop();

private:
  TransferReceiver(const TransferReceiver&);
  TransferReceiver& operator=(const TransferReceiver&);

  void get_next_chunk(Message*);

  MessageHandler<TLSSocket>* client;
  std::string id;
  boost::filesystem::path filename;
  int length;
  int received_length;
  std::function<void(const TransferReceiver*)> end_callback;
  boost::filesystem::ofstream file;
};

#endif // TRANSFER_RECEIVER_HPP

/**@}*/
