/** @addtogroup Network
 *  @{
 */

/**
 * Represent an outgoing file transfer.
 * @class TransferReceiver
 */

#ifndef __TRANSFER_RECEIVER_HPP__
# define __TRANSFER_RECEIVER_HPP__

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

#include <network/message.hpp>

class Client;

class TransferReceiver
{
public:
  TransferReceiver(Client*, const std::string&, const std::string&, int);
  ~TransferReceiver();
  void stop();

private:
  TransferReceiver(const TransferReceiver&);
  TransferReceiver& operator=(const TransferReceiver&);

  void get_next_chunk(Message*);
  Client* client;
  std::string id;
  fs::path filename;
  int length;
  int received_length;
  fs::ofstream file;
};

#endif // __TRANSFER_RECEIVER_HPP__

/**@}*/
