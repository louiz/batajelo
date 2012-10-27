#include <string>

#ifndef __GUI_LAYER_HPP__
# define __GUI_LAYER_HPP__

class Camera;
class GraphMap;

class Layer
{
  friend class Camera;
  friend class GraphMap;
public:
  Layer();
  ~Layer();
  void set_size(unsigned int width, unsigned int height);
  void set_level(unsigned int level);
  unsigned int get_level() const;
  void set_cell(unsigned int cell, const unsigned int value);
  unsigned int get_cell(const unsigned int cell) const;

private:
  unsigned int width;
  unsigned int height;
  unsigned int* cells;
  unsigned int level;
};

#endif // __GUI_LAYER_HPP__
