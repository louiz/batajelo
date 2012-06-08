#include <string>

#ifndef __GUI_LAYER_HPP__
# define __GUI_LAYER_HPP__

#include <config/config.hpp>
#include <logging/logging.hpp>

class Layer
{
public:
  Layer();
  ~Layer();
  void set_size(unsigned int width, unsigned int height);
  void set_level(unsigned int level);
  void set_cell(unsigned int cell, const unsigned int value);
  unsigned int get_cell(const unsigned int cell) const;

private:
  unsigned int width;
  unsigned int height;
  unsigned int* cells;
  unsigned int level;
};

#endif // __GUI_LAYER_HPP__
