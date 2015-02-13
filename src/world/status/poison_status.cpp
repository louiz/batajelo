#include <world/status/poison_status.hpp>

#include <world/world.hpp>
#include <world/entity.hpp>

#include <world/health.hpp>

PoisonStatus::PoisonStatus(Entity* entity, World* world):
  Status(entity, world)
{
}

void PoisonStatus::tick()
{
  Health* health = this->entity->get<Health>();
  if (health)
    health->add(-0.1_fix);
}
