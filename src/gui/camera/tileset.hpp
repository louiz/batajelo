#ifndef TILESET_HPP_INCLUDED
#define TILESET_HPP_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

static const std::string MINIMAPS_DIRECTORY("./data/minimaps/");
static const std::string TILESETS_DIRECTORY("./data/tilesets/");

#include <gui/camera/tile.hpp>
#include <world/map.hpp>

class Camera;
class Minimap;

class Tileset: public Map
{
  friend class Camera;
  friend class Minimap;
public:
  Tileset();
  ~Tileset();
  bool load_from_file(const std::string& filename);
  /**
   * Draw the full map on the given render target.
   * Used to generate, e.g., a minimap.
   */
  void draw_full_map(sf::RenderTarget&);

private:
  Tileset(const Tileset&);
  Tileset& operator=(const Tileset&);

  bool read_tileset(const boost::property_tree::ptree& tree);
  /**
   * The list of all the tiles used in this map. The position in the list
   * correspond to the gid value en the layer's cells.
   * The gid 0 reprents no sprite, hence tiles[0] is set to NULL.
   */
  std::vector<std::unique_ptr<GraphTile>> tiles;
  /**
   * A list of all the textures used to create tile sprites.
   * We keep them in memory even if we don't use them anymore
   * after the sprite creation, because sfml requires the
   * texture pointer to remain valid.
   */
  std::vector<std::unique_ptr<sf::Texture>> tileset_textures;
  sf::Texture minimap_texture;
};

#endif /* TILESET_HPP_INCLUDED */
