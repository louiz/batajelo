/** @addtogroup Network
 *  @{
 */

/**
 * Represents one single network message, for the base network protocol.
 * A message has a header and a body (also called binary part, which can have
 * size of 0).
 * The header is MESSAGE_NAME.BODY_SIZE:
 * The body can be anything, but it is BODY_SIZE bytes long.
 * The \n char is not used anywhere in the protocol, and there's no delimiter
 * between a body and the header of the next message.
 *
 * To send a message, a Message object must be created anywhere and filled with
 * the correct data and then passed to the MessageHandler::send() method.
 * The object will be deleted by the send_handler, after it has been
 * successfully sent.
 *
 * A Message object is passed by a MessageHandler to the callback associated
 * with a message name. This callback is responsible for deleting the message
 * object. (MAYBE)
 * @class Message
 */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <google/protobuf/message.h>
#include <serialization/exception.hpp>

#include "logging/logging.hpp"

#include <functional>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <functional>

class Message
{
public:
  Message();
  Message(const Message&);
  ~Message();
  /**
   * Sets the body of the message. A char* will be new[]ed using the size, and
   * the data will be copied in it. To avoid that copy, see the body attribute
   * and the set_data() method. You should use one of the two methods on an
   * object, not both.
   */
  void set_body(const char*, int size = -1);
  /**
   * Sets the body of the message with the content of given the
   * protobuf::Message, serialized.
   */
  void set_body(const google::protobuf::Message& msg);
  /**
   * If you manually set the content of the body member, use this method to
   * set the proper body size. Do not use it after set_body() though.
   */
  void set_body_size(int size);
  /**
   * This must be called before the object is passed to MessageHandler::send(),
   * it will set the header correctly, using the body size etc.
   */
  void pack();
  void set_name(const std::string& name);
  std::string get_name() { return this->name; }
  /**
   * Use this member to manually set the body. For example you can pass it to
   * a ostream::get() method after having manually new[]ed it. This avoid
   * copying data twice.
   */
  char* body;
  size_t body_size;
  /**
   * If not empty, will be called from the send_handler.
   */
  std::function<void(void)> callback;
  std::string header;
  std::string name;
  /**
   * Converts the body to a protobuf object. Does not check if it's valid
   * nor complete.
   */
  template<typename ProtobufClass>
  ProtobufClass parse_body_to_protobuf_object() const
  {
    ProtobufClass res;
    res.ParseFromArray(this->body, this->body_size);
    log_debug("parse_body_to_protobuf_object: " << res.ShortDebugString());
    if (!res.IsInitialized())
      throw SerializationException{res.InitializationErrorString()};
    return res;
  }

private:
  Message& operator=(const Message&);
};

#endif // __MESSAGE_HPP__
