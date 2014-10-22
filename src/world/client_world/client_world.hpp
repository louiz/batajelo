#ifndef __CLIENT_WORLD_HPP__
# define __CLIENT_WORLD_HPP__

#include <world/world.hpp>
#include <game/selection.hpp>

class Camera;

class ClientWorld: public World
{
  friend class Screen;
public:
  ClientWorld(Map*, Mod&);
  virtual ~ClientWorld();
  void init();

private:
  ClientWorld(const ClientWorld&);
  ClientWorld& operator=(const ClientWorld&);
  Selection current_selection;
};

#endif // __CLIENT_WORLD_HPP__
