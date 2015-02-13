#ifndef POISON_STATUS_HPP_INCLUDED
#define POISON_STATUS_HPP_INCLUDED

#include <world/status.hpp>

class PoisonStatus: public Status
{
public:
  PoisonStatus(Entity* entity, World* world);
  ~PoisonStatus() = default;
   void tick() override final;

private:
  PoisonStatus(const PoisonStatus&) = delete;
  PoisonStatus(PoisonStatus&&) = delete;
  PoisonStatus& operator=(const PoisonStatus&) = delete;
  PoisonStatus& operator=(PoisonStatus&&) = delete;
};


#endif /* POISON_STATUS_HPP_INCLUDED */
