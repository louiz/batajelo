#ifndef __VIEWER_HPP__
# define __VIEWER_HPP__

#include <world/occupant.hpp>

class Viewer: public Occupant
{
public:
  Viewer(unsigned long int number, const std::string& name);
  Viewer();
  ~Viewer();

  virtual void serialize(oarchive & ar, const unsigned int version)
  {
    Occupant::serialize(ar, version);
  }
  virtual void serialize(iarchive & ar, const unsigned int version)
  {
    Occupant::serialize(ar, version);
  }

private:
  Viewer(const Viewer&);
  Viewer& operator=(const Viewer&);
};

#endif // __VIEWER_HPP__
