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

#define FILES_TO_RECEIVE_DIRECTORY std::string("./receive/")

#include <logging/logging.hpp>
#include <network/command.hpp>

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

  void get_next_chunk(Command*);
  Client* client;
  std::string id;
  const std::string filename;
  int length;
  int received_length;
  std::ofstream file;
};

#endif // __TRANSFER_RECEIVER_HPP__

/**@}*/
