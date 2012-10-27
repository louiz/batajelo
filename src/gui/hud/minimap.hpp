/** @addtogroup Ui
 *  @{
 */

/**
 * Display the complete map, at a very big scale, along with the entities
 * and camera positions.
 * @class Minimap
 */

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#ifndef __MINIMAP_HPP__
# define __MINIMAP_HPP__

#define MINIMAP_X 35.f
#define MINIMAP_Y 20.f
#define MINIMAP_WIDTH 250.f
#define MINIMAP_HEIGHT 250.f

#include <gui/camera/map.hpp>
#include <gui/camera/camera.hpp>

class Minimap
{
public:
  Minimap(sf::RenderWindow*, GraphMap*, Camera*);
  ~Minimap();
  void draw();
  bool handle_event(const sf::Event&);

private:
  Minimap(const Minimap&);
  Minimap& operator=(const Minimap&);
  sf::RenderWindow* win;
  GraphMap* map;
  Camera* camera;
  sf::Sprite minimap_sprite;
};

#endif // __MINIMAP_HPP__
