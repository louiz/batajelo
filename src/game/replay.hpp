/** @addtogroup Game
 *  @{
 */

/**
 * A replay is just a serie a network::Message, each containing a body with
 * some serialized order object.
 */

#ifndef REPLAY_HPP_INCLUDED
#define REPLAY_HPP_INCLUDED

#include <memory>
#include <list>

#include <network/message.hpp>

class Replay
{
public:
  Replay();
  ~Replay();
  /**
   * Copy and insert an order at the end of the list.
   */
  void insert_order(const Message&);
  std::list<Message>& get_orders();

private:
  std::list<Message> orders;

  Replay(const Replay&);
  Replay& operator=(const Replay&);
};

#endif /* REPLAY_HPP_INCLUDED */

