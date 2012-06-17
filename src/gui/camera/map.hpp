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

#define MAPS_DIRECTORY "./data/maps/"
#define MINIMAPS_DIRECTORY "./data/minimaps/"
#define TILESETS_DIRECTORY "./data/tilesets/"

#define LAYER_NUMBER 5
#define LEVEL_HEIGHT 32
#define TILE_WIDTH 128
#define TILE_HEIGHT 96

#include <logging/logging.hpp>
#include <gui/camera/layer.hpp>
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
  virtual bool load_from_file(const std::string& filename, bool load_minimap = true);
  uint get_height_in_pixels() const;
  uint get_width_in_pixels() const;
  uint get_height_in_tiles() const;
  uint get_width_in_tiles() const;

  void reset_layers_iterator();
  Layer* get_next_layer();
  /**
   * Draw the full map on the given render target.
   * Used to generate, e.g., a minimap.
   */
  void draw_full_map(sf::RenderTarget&);

private:
  GraphMap(const GraphMap&);
  GraphMap& operator=(const GraphMap&);

  bool read_layer(boost::property_tree::ptree& tree);
  bool read_tileset(boost::property_tree::ptree& tree);
  bool get_layer_level(boost::property_tree::ptree& tree, unsigned int& level);
  bool get_layer_data(boost::property_tree::ptree& tree, std::string& data);
  /**
   * The width (in pixels) of the map.
   */
  uint width;
  /**
   * The height (in pixels) of the map.
   */
  uint height;
  /**
   * The number of tiles, horizontally.
   */
  uint width_in_tiles;
  /**
   * The number of tiles, vertically.
   */
  uint height_in_tiles;
  /**
   * The list of all layers. A layer can be NULL.
   */
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

  std::vector<Layer*>::const_iterator layers_iterator;
  sf::Texture minimap_texture;
};

#endif // __UI_MAP_HPP__
