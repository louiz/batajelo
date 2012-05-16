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

class Event: public Serializable
{
public:
  Event() {}
  ~Event() {}

private:
  Event(const Event&);
  Event& operator=(const Event&);
};

class MoveEvent: public Event
{
public:
  MoveEvent() {}
  ~MoveEvent() {}

  std::vector<unsigned short> actors_ids;
  unsigned int x;
  unsigned int y;

  virtual void serialize(boost::archive::text_oarchive & ar, const unsigned int)
  {
    ar & this->x;
    ar & this->y;
    ar & this->actors_ids;
  }
  virtual void serialize(boost::archive::text_iarchive & ar, const unsigned int)
  {
    ar & this->x;
    ar & this->y;
    ar & this->actors_ids;
  }
private:
  MoveEvent(const MoveEvent&);
  MoveEvent& operator=(const MoveEvent&);
};

#endif // __EVENT_HPP__
/**@}*/
