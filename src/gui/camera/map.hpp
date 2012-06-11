#include <zlib.h>

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

#define LAYER_NUMBER 5

#define TILE_WIDTH 128
#define TILE_HEIGHT 96

#include <logging/logging.hpp>
#include <gui/camera/layer.hpp>
#include <gui/camera/tile.hpp>

class GraphMap
{
public:
  GraphMap();
  ~GraphMap();
  bool load_from_file(const std::string& filename);
  void set_size();
  bool read_layer(boost::property_tree::ptree& tree);
  bool read_tileset(boost::property_tree::ptree& tree);
  bool get_layer_level(boost::property_tree::ptree& tree, unsigned int& level);
  bool get_layer_data(boost::property_tree::ptree& tree, std::string& data);

private:
  GraphMap(const GraphMap&);
  GraphMap& operator=(const GraphMap&);
  std::vector<Layer*> layers;
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
};

#endif // __UI_MAP_HPP__