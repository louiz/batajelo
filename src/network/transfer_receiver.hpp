/** @addtogroup Network
 *  @{
 */

/**
 * Represent an outgoing file transfer.
 * @class TransferReceiver
 */

#include <iostream>
#include <fstream>
#include <string>

#ifndef __TRANSFER_RECEIVER_HPP__
# define __TRANSFER_RECEIVER_HPP__

#include <logging/logging.hpp>

class Client;

class TransferReceiver
{
public:
  TransferReceiver(Client*, const std::string&, const std::string&, int);
  ~TransferReceiver();

private:
  TransferReceiver(const TransferReceiver&);
  TransferReceiver& operator=(const TransferReceiver&);

  void get_next_chunk(const char*, int);
  Client* client;
  std::string id;
  const std::string filename;
  int length;
  int received_length;
  std::ofstream file;
};

#endif // __TRANSFER_RECEIVER_HPP__

/**@}*/
