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

#include <world/entity.hpp>
#include <serialization/serializable.hpp>
#include <network/command.hpp>

class Event: public Serializable
{
public:
  Event(const unsigned long int id);
  Event(const Command*);
  Event();
  virtual ~Event() {}
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
  bool is_valid() const
  {
    return this->valid;
  }

private:
  Event(const Event&);
  Event& operator=(const Event&);

protected:
  unsigned long int id;
  bool valid;
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

/**
 * Just an event containing a turn number, saying that
 * the action generated by this event must be scheduled for this turn.
 */
class ActionEvent: public Event
{
public:
  ActionEvent(const std::string& name):
    Event(),
    name(name)
  {}
  ActionEvent(const Command*);

  virtual void serialize(boost::archive::text_oarchive& ar, const unsigned int v)
  {
    Event::serialize(ar, v);
    ar & turn;
  }
  virtual void serialize(boost::archive::text_iarchive& ar, const unsigned int v)
  {
    Event::serialize(ar, v);
    ar & turn;
  }
  unsigned long int turn;
  const std::string name;
private:
  ActionEvent(const ActionEvent&);
  ActionEvent& operator=(const ActionEvent&);
};

/**
 * An event containing an entity object.
 */
class EntityEvent: public ActionEvent
{
public:
  EntityEvent(Entity* entity):
    ActionEvent("NEW_ENTITY"),
    entity(entity)
  {}
  EntityEvent(const Command*);

  virtual void serialize(boost::archive::text_oarchive& ar, const unsigned int v)
  {
    ActionEvent::serialize(ar, v);
    ar & entity;
  }
  virtual void serialize(boost::archive::text_iarchive& ar, const unsigned int v)
  {
    ActionEvent::serialize(ar, v);
    ar & entity;
  }
  Entity* entity;

private:
  EntityEvent(const EntityEvent&);
  EntityEvent& operator=(const ActionEvent&);
};

class MoveEvent: public Event
{
public:
  MoveEvent(): Event() {}
  MoveEvent(const Command*);
  virtual void serialize(boost::archive::text_oarchive& ar, const unsigned int v)
  {
    Event::serialize(ar, v);
    ar & actors_ids & x & y;
  }
  virtual void serialize(boost::archive::text_iarchive& ar, const unsigned int v)
  {
    Event::serialize(ar, v);
    ar & actors_ids & x & y;
  }

  std::vector<unsigned short> actors_ids;
  unsigned int x;
  unsigned int y;

private:
  MoveEvent(const MoveEvent&);
  MoveEvent& operator=(const MoveEvent&);
};

class PathEvent: public ActionEvent
{
public:
  PathEvent():
    ActionEvent("PATH")
  {}

  virtual void serialize(boost::archive::text_oarchive& ar, const unsigned int v)
  {
    ActionEvent::serialize(ar, v);
    ar & actors_ids & x & y;
  }
  virtual void serialize(boost::archive::text_iarchive& ar, const unsigned int v)
  {
    ActionEvent::serialize(ar, v);
    ar & actors_ids & x & y;
  }

  PathEvent(const Command*);
  PathEvent(const MoveEvent& e):
    ActionEvent("PATH"),
    actors_ids(e.actors_ids),
    x(e.x),
    y(e.y)
  {}
  std::vector<unsigned short> actors_ids;
  unsigned int x;
  unsigned int y;
};

#endif // __EVENT_HPP__
/**@}*/
