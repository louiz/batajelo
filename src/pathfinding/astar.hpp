#ifndef __ASTAR_HPP__
# define __ASTAR_HPP__

#include <logging/logging.hpp>

/**
 * Takes a walking_map, a map width (in number of cells)
 */
void do_astar(const ushort* map, const uint map_width,
              const uint startx, const uint starty,
              const uint endx, const uint endy);

#endif // __ASTAR_HPP__
