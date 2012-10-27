#include <world/layer.hpp>

Layer::Layer():
  cells(0)
{
}

Layer::~Layer()
{
  if (this->cells)
    delete[] this->cells;
}

void Layer::set_size(unsigned int width, unsigned int height)
{
  this->width = width;
  this->height = height;
  if (this->cells)
    delete [] this->cells;
  this->cells = new unsigned int[width*height];
}

void Layer::set_level(const unsigned int level)
{
  this->level = level;
}

unsigned int Layer::get_level() const
{
  return this->level;
}

void Layer::set_cell(const unsigned int cell, const unsigned int value)
{
  this->cells[cell] = value;
}

unsigned int Layer::get_cell(const unsigned int cell) const
{
  if (this->cells == 0)
    return 0;
  return this->cells[cell];
}
