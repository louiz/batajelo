#ifndef __SERIALIZABLEENTITY_HPP__
# define __SERIALIZABLEENTITY_HPP__

#include <logging/logging.hpp>
#include <serialization/serializable.hpp>
#include <world/entity.hpp>
#include <world/path.hpp>
#include <string>

class SerializableEntity: Serializable
{
public:
  SerializableEntity();
  SerializableEntity(const Entity& e);
  virtual ~SerializableEntity();

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & x & y & type_id;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
   ar & x & y & type_id;
  }

private:
  SerializableEntity& operator=(const SerializableEntity&);
  SerializableEntity(const SerializableEntity&);
public:
  /**
   * The left position
   */
  std::string x;
  /**
   * The top position
   */
  std::string y;
  /**
   * A uniq id for the entity type.
   */
  unsigned short type_id;
};

#endif // __ENTITY_HPP__
