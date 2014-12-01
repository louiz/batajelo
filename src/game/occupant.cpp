#include <game/occupant.hpp>

Occupant::Occupant(uint32_t id, const std::string& name):
  id(id),
  name(name)
{
}

Occupant::Occupant(const ser::game::Occupant& srl):
  id(srl.id()),
  name(srl.name())
{
}

Occupant::~Occupant()
{
}

ser::game::Occupant Occupant::serialize() const
{
  ser::game::Occupant srlzed;
  srlzed.set_id(this->id);
  srlzed.set_name(this->name);
  return srlzed;
}

