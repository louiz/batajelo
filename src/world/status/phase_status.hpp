#ifndef PHASE_STATUS_HPP_INCLUDED
#define PHASE_STATUS_HPP_INCLUDED

#include <world/status.hpp>

class PhaseStatus: public Status
{
public:
  PhaseStatus(Entity* entity, World* world);
  ~PhaseStatus() = default;
  void apply() override final;
  void unapply() override final;

private:
  PhaseStatus(const PhaseStatus&) = delete;
  PhaseStatus(PhaseStatus&&) = delete;
  PhaseStatus& operator=(const PhaseStatus&) = delete;
  PhaseStatus& operator=(PhaseStatus&&) = delete;
};


#endif /* PHASE_STATUS_HPP_INCLUDED */
