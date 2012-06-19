/**
 * A class containing all informations to render the map onscreen.
 */

#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <SFML/Graphics.hpp>

#ifndef __UI_MAP_HPP__
# define __UI_MAP_HPP__

#define MINIMAPS_DIRECTORY "./data/minimaps/"
#define TILESETS_DIRECTORY "./data/tilesets/"

#include <logging/logging.hpp>
#include <gui/camera/tile.hpp>
#include <world/map.hpp>

class Camera;
class Minimap;

class GraphMap: public Map
{
  friend class Camera;
  friend class Minimap;
public:
  GraphMap();
  ~GraphMap();
  bool load_from_file(const std::string& filename, bool load_minimap = true);
  /**
   * Draw the full map on the given render target.
   * Used to generate, e.g., a minimap.
   */
  void draw_full_map(sf::RenderTarget&);

private:
  GraphMap(const GraphMap&);
  GraphMap& operator=(const GraphMap&);

  bool read_tileset(boost::property_tree::ptree& tree);
  /**
   * The list of all the tiles used in this map. The position in the list
   * correspond to the gid value en the layer's cells.
   * The gid 0 reprents no sprite, hence tiles[0] is set to NULL.
   */
  std::vector<GraphTile*> tiles;
  /**
   * A list of all the textures used to create tile sprites.
   * We keep them in memory even if we don't use them anymore
   * after the sprite creation, because sfml requires the
   * texture pointer to remain valid.
   */
  std::vector<sf::Texture*> tileset_textures;
  sf::Texture minimap_texture;
};

#endif // __UI_MAP_HPP__
