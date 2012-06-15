#include <SFML/Graphics.hpp>

#ifndef __GRAPH_TILE_HPP__
# define __GRAPH_TILE_HPP__

class Camera;

class GraphTile
{
  friend Camera;
public:
  GraphTile(const sf::Texture&, const sf::IntRect&);
  ~GraphTile();

private:
  GraphTile(const GraphTile&);
  GraphTile& operator=(const GraphTile&);
  sf::Sprite sprite;
};

#endif // __GRAPH_TILE_HPP__
