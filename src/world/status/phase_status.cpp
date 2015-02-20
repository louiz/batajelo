#include <world/status/phase_status.hpp>
#include <world/mobility.hpp>
#include <world/entity.hpp>

#include <logging/logging.hpp>

#include <cassert>

PhaseStatus::PhaseStatus(Entity* entity, World* world):
  Status(entity, world)
{
  log_debug("Creating Phase for entity " << entity->get_id());
}

void PhaseStatus::apply()
{
  log_debug("applying");
  Mobility* mobility = this->entity->get<Mobility>();
  assert (mobility);
    mobility->set_speed(mobility->get_speed() + 0.8_fix);
}

void PhaseStatus::unapply()
{
  Mobility* mobility = this->entity->get<Mobility>();
  assert (mobility);
    mobility->set_speed(mobility->get_speed() - 0.8_fix);
}
