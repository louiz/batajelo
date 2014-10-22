#include <logging/logging.hpp>
#include <world/client_world/client_world.hpp>
#include <gui/camera/camera.hpp>
#include <gui/sprites/sprite.hpp>

ClientWorld::ClientWorld(Map* map, Mod& mod):
  World(map, mod),
  camera(0)
{
}

ClientWorld::~ClientWorld()
{
}
