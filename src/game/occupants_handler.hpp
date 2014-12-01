#ifndef OCCUPANTS_HANDLER_INCLUDED_HPP
#define OCCUPANTS_HANDLER_INCLUDED_HPP

#include <game/occupant.hpp>
#include <memory>
#include <vector>

using OccupantsList = std::vector<std::unique_ptr<Occupant>>;

class OccupantsHandler
{
public:
  OccupantsHandler();
  ~OccupantsHandler();
  void add(OccupantsList::value_type&& occupant);
  void remove(const OccupantsList::const_iterator it);
  void remove(const OccupantsList::value_type& occupant);
  void remove(const OccupantsList::value_type::element_type& occupant);
  OccupantsList::iterator erase(const OccupantsList::const_iterator begin,
                                const OccupantsList::const_iterator end);
  OccupantsList::size_type size() const;
  OccupantsList::iterator begin();
  OccupantsList::iterator end();
  OccupantsList::const_iterator begin() const;
  OccupantsList::const_iterator end() const;


private:
  std::vector<std::unique_ptr<Occupant>> occupants;

  OccupantsHandler(const OccupantsHandler&) = delete;
  OccupantsHandler(OccupantsHandler&&) = delete;
  OccupantsHandler& operator=(const OccupantsHandler&) = delete;
  OccupantsHandler& operator=(OccupantsHandler&&) = delete;
};

#endif /* OCCUPANTS_HANDLER_INCLUDED_HPP */
