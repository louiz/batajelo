/** @addtogroup Game
 *  @{
 */

/**
 * Structures containing the information associated with
 * a player event. For example the MOVE event is associated
 * with a list of entity IDs, and a destination.
 * @class Event
 */

#ifndef __EVENT_HPP__
# define __EVENT_HPP__

#include <serialization/serializable.hpp>
#include <network/command.hpp>

class Event: public Serializable
{
public:
  Event(const unsigned long int id);
  Event(const Command*);
  Event();
  ~Event() {}
  static unsigned long int current_id;
  virtual void serialize(boost::archive::text_oarchive& ar, const unsigned int)
  {
    ar & id;
  }
  virtual void serialize(boost::archive::text_iarchive& ar, const unsigned int)
  {
    ar & id;
  }

  unsigned long int get_id() const
  {
    return this->id;
  }

private:
  Event(const Event&);
  Event& operator=(const Event&);

  unsigned long int id;
};

/**
 * Event used by the server to tell a client that the action (id)
 * has been validated by the client (client_id)
 */
class OkEvent: public Event
{
public:
  OkEvent(const unsigned int id,
	  const unsigned long int client_id):
    Event(id),
    client_id(client_id)
  {}
  OkEvent(const Command*);
  virtual void serialize(boost::archive::text_oarchive& ar, const unsigned int v)
  {
    Event::serialize(ar, v);
    ar & client_id;
  }
  virtual void serialize(boost::archive::text_iarchive& ar, const unsigned int v)
  {
    Event::serialize(ar, v);
    ar & client_id;
  }
  unsigned long int client_id;
};

#endif // __EVENT_HPP__
/**@}*/
