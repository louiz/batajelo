#include <world/layer.hpp>
#include <logging/logging.hpp>
Layer::Layer():
  cells(nullptr)
{
}

Layer::~Layer()
{
  if (this->cells)
    delete[] this->cells;
}

void Layer::set_size(unsigned int width, unsigned int height)
{
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
  if (!this->cells)
    return 0;
  return this->cells[cell];
}
