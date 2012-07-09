#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#ifndef __MAP_HPP__
# define __MAP_HPP__

#include <logging/logging.hpp>
#include <world/layer.hpp>

#define LAYER_NUMBER 5
#define LEVEL_HEIGHT 32
#define TILE_WIDTH 128
#define TILE_HEIGHT 96

// In tiles.
#define TILESET_WIDTH 8
#define TILESET_HEIGHT 8

#define MAPS_DIRECTORY "./data/maps/"

/**
 * Defines the heights of the four corners of various tiles based on where
 * they are positionned in the tile set.  For example a tile positionned on
 * the second line of the tileset has these heights: left-top=0,
 * right-top=1, bottom-right=1, bottom-left=0, bottom=1.
 */
static const char tile_heights[6][4] =
{
  {1, 1, 1, 1},
  {0, 1, 1, 0},
  {1, 0, 0, 1},
  {0, 0, 1, 1},
  {1, 1, 0, 0},
  {0, 0, 0, 0}
};

class Camera;

class Map
{
  friend class Camera;
public:
  Map();
  virtual ~Map();
  bool load_from_file(const std::string&);
  uint get_height_in_pixels() const;
  uint get_width_in_pixels() const;
  uint get_height_in_tiles() const;
  uint get_width_in_tiles() const;
  void reset_layers_iterator();
  Layer* get_next_layer();
  int get_max_level_for_cell(const uint cell) const;
  /**
   * Generates the walking map. This structure is used to know the height of
   * an entity, based on its position, and to do the path finding (it
   * describes the traversability from one tile to another based on these
   * tiles' heights).
   */
  void generate_walking_map();
  /**
   * Returns an ushort containing the 4 corners' heights of the given cell.
   */
  ushort get_cell_heights(const int cellx, const int celly) const;

protected:
  bool read_layer(boost::property_tree::ptree& tree);
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
  std::vector<Layer*>::const_iterator layers_iterator;
  /**
   * A representation of the map, containing 4 heights for each tile (one
   * for each corner).
   * The heights in each cell is based on the tile_heights value + the level
   * value.
   */
   ushort* walking_map;

private:
  Map(const Map&);
  Map& operator=(const Map&);
};

#endif // __MAP_HPP__
