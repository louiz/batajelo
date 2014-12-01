#include <game/occupants_handler.hpp>
#include <logging/logging.hpp>

#include <algorithm>

OccupantsHandler::OccupantsHandler()
{
}

OccupantsHandler::~OccupantsHandler()
{
}

void OccupantsHandler::add(OccupantsList::value_type&& occupant)
{
  log_debug("Adding new occupant to the world:" << occupant->name << " " << occupant->id);
  this->occupants.push_back(std::move(occupant));
}

void OccupantsHandler::remove(const OccupantsList::const_iterator it)
{
  this->occupants.erase(it);
}

OccupantsList::iterator OccupantsHandler::erase(const OccupantsList::const_iterator begin,
                                                const OccupantsList::const_iterator end)
{
  return this->occupants.erase(begin, end);
}

void OccupantsHandler::remove(const OccupantsList::value_type& occupant)
{
  this->occupants.erase(std::remove_if(this->occupants.begin(), this->occupants.end(),
                                       [&occupant](const auto& occ){
                                         return occ->id == occupant->id;
                                       }),
                        this->occupants.end());
}

void OccupantsHandler::remove(const OccupantsList::value_type::element_type& occupant)
{
  this->occupants.erase(std::remove_if(this->occupants.begin(), this->occupants.end(),
                                       [&occupant](const auto& occ){
                                         return occ->id == occupant.id;
                                       }),
                        this->occupants.end());
}

OccupantsList::size_type OccupantsHandler::size() const
{
  return this->occupants.size();
}

OccupantsList::iterator OccupantsHandler::begin()
{
  return this->occupants.begin();
}
OccupantsList::iterator OccupantsHandler::end()
{
  return this->occupants.begin();
}

OccupantsList::const_iterator OccupantsHandler::begin() const
{
  return this->occupants.begin();
}

OccupantsList::const_iterator OccupantsHandler::end() const
{
  return this->occupants.end();
}
