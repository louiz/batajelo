/** @addtogroup Network
 *  @{
 */

/**
 * Represent an outgoing file transfer. The file is sent by chunks, each in a
 * different message. The transfer has an ID that is used in the message's
 * command name, that way the receiver just needs to add a callback on that
 * ID to receive all the chunks for that specific file. Every chunk is sent as
 * soon as the previous one was correctly sent on the socket.
 * Two entities (e.g. one client with one server) can have any number of
 * simultaneous file transfers, without any confilct or delay. And a server
 * can send files to any number of clients.
 * @class TransferSender
 */

#include <string>
#include <boost/asio.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

class TLSSocket;

template <typename SocketType>
class MessageHandler;

#ifndef __TRANSFER_SENDER_HPP__
# define __TRANSFER_SENDER_HPP__

static const boost::filesystem::path FILES_TO_SEND_DIRECTORY("./send/");

class TransferSender
{
public:
  static unsigned long int current_id;
  TransferSender(MessageHandler<TLSSocket>* client, const std::string&,
                 std::function<void(const TransferSender*)>&& end_callback);
  ~TransferSender();
  /**
   * Try to start the file transfer, returns true if there was
   * no error opening the file and reading, false otherwise (in
   * that case this object should immediately be deleted by the RemoteClient
   * object.
   */
  bool start();
  void send_next_chunk();

private:
  TransferSender(const TransferSender&);
  TransferSender& operator=(const TransferSender&);

  MessageHandler<TLSSocket>* client;
  const std::string filename;
  std::function<void(const TransferSender*)> end_callback;

  boost::filesystem::ifstream file;
  int length;
  std::string id;
};

#endif // __TRANSFER_SENDER_HPP__
/**@}*/
