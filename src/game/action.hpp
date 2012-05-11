/** @addtogroup Game
 *  @{
 */

/**
 * An action, associated with a turn, kept in a turn_handler, with a
 * callback executed when the turn is passed (see TurnHandler).
 * @class Action
 */

#include <boost/function.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>

#ifndef __ACTION_HPP__
# define __ACTION_HPP__

#include <logging/logging.hpp>

namespace actions
{
  enum Type
    {
      Select,
      Move,
      Attack,
      Cast,

      None
    };
}

class Action
{
  friend class boost::serialization::access;
public:
  Action(actions::Type type);
  Action():type(actions::None) {};
  ~Action();
  void execute() const;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int)
  {
    ar & this->type;
    ar & this->x;
    ar & this->y;
    ar & this->actors_ids;
    ar & this->targets_ids;
  }

  actions::Type type;
  std::vector<unsigned short> actors_ids;
  std::vector<unsigned short> targets_ids;
  unsigned int x;
  unsigned int y;

private:
  Action(const Action&);
  Action& operator=(const Action&);
};

#endif // __ACTION_HPP__
/**@}*/
