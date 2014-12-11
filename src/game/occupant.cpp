#include <game/occupant.hpp>

Occupant::Occupant(uint32_t id, const std::string& name):
  id(id),
  name(name)
{
}

Occupant::Occupant(const ser::game::Occupant& srl):
  id(srl.id()),
  name(srl.name()),
  team(srl.team())
{
}

Occupant::~Occupant()
{
}

void Occupant::set_team(const uint16_t team)
{
  this->team = team;
}

uint16_t Occupant::get_team() const
{
  return this->team;
}

ser::game::Occupant Occupant::serialize() const
{
  ser::game::Occupant srlzed;
  srlzed.set_id(this->id);
  srlzed.set_name(this->name);
  srlzed.set_team(this->team);
  return srlzed;
}

