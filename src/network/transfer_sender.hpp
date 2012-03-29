/** @addtogroup Network
 *  @{
 */

#include <iostream>
#include <fstream>
#include <string>

#ifndef __TRANSFER_SENDER_HPP__
# define __TRANSFER_SENDER_HPP__

#include <logging/logging.hpp>

class RemoteClient;

class TransferSender
{
public:
  static unsigned long int current_id;
  TransferSender(RemoteClient*, const std::string&);
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

  const std::string filename;
  RemoteClient* client;
  std::ifstream file;

  int length;
  unsigned long int id;
};

#endif // __TRANSFER_SENDER_HPP__

/**@}*/
