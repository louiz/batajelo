/** @addtogroup Network
 *  @{
 */

/**
 * Represents one single network command.
 * A command has a header and a body (also called binary part, which can have
 * size of 0).
 * The header is COMMAND_NAME.BODY_SIZE:
 * The body can be anything, but it is BODY_SIZE bytes long.
 * The \n char is not used anywhere in the protocol, and there's no delimiter
 * between a body and the header of the next message.
 *
 * To send a command, a Command object must be created anywhere and filled with
 * the correct data and then passed to the CommandHandler::send() method.
 * The object will be deleted by the send_handler, after it has been
 * succesfully sent.
 *
 * A Command object is passed by a CommandHandler to the callback associated
 * with a command name. This callback is responsible for deleting the command
 * object. (MAYBE)
 * @class Command
 */

#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

#ifndef __COMMAND_HPP__
# define __COMMAND_HPP__

#include <logging/logging.hpp>

class Command
{
public:
  Command();
  ~Command();
  /**
   * Sets the body of the message. A char* will be new[]ed using the size, and
   * the data will be copied in it. To avoid that copy, see the body attribute
   * and the set_data() method. You should use one of the two methods on an
   * object, not both.
   */
  void set_body(const char*, int size = -1);
  /**
   * If you manually set the content of the body member, use this method to
   * set the proper body size. Do not use it after set_body() though.
   */
  void set_body_size(int size);
  /**
   * This must be called before the object is passed to CommandHandler::send(),
   * it will set the header correctly, using the body size etc.
   */
  void pack();
  void set_name(const std::string);
  std::string get_name() { return this->name; }
  /**
   * Use this member to manually set the body. For example you can pass it to
   * a ostream::get() method after having manually new[]ed it. This avoid
   * copying data twice.
   */
  char* body;
  std::string header;
  std::string name;
  size_t body_size;

private:
  Command(const Command&);
  Command& operator=(const Command&);
};

#endif // __COMMAND_HPP__
