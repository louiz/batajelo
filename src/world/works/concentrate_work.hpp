#ifndef CONCENTRATE_WORK_HPP_INCLUDED
#define CONCENTRATE_WORK_HPP_INCLUDED

#include <world/works/single_task_work.hpp>
#include <world/tasks/concentrate_task.hpp>

/**
 * A work, similar to channeling on dota 2 (http://dota2.gamepedia.com/Channeling).
 */

using ConcentrateWork = SingleTaskWork<ConcentrateTask>;

#endif
