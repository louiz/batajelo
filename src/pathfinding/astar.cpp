#include <pathfinding/astar.hpp>

void do_astar(const ushort* map, const uint map_width,
              const uint cell_width, const uint cell_height,
              const uint startx, const uint starty,
              const uint endx, const uint endy)
{
}

void get_cell_at_position(const uint cell_size,
                          const uint x, const uint y,
                          int& xa, int& ya)
{
  assert(cell_size % 2 == 0);
  assert(cell_size > 1);
  xa = x / cell_size;
  ya = y / (cell_size / 2);

  if ((x % cell_size) < (cell_size / 2 - 1))
    {
      if ((y % cell_size) > (cell_size / 2))
        {
          if ((y % (cell_size / 2)) > (x % (cell_size / 2)))
            xa = xa - 1;
        }
      else
        {
          if ((y % (cell_size / 2)) < (x % (cell_size / 2)))
            xa = xa - 1;
        }
    }
if (ya % 2)
  {
    if ((y % (cell_size / 2)) < ((cell_size / 2) - x % (cell_size / 2)))
      ya--;
  }
 else
   {
     if ((y % (cell_size / 2)) < (x % (cell_size / 2)))
       ya--;
   }
  log_debug(xa << ":" << ya);
}
