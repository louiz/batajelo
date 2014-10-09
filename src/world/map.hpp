#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <list>
#include <tuple>
#include <limits>

#ifndef __MAP_HPP__
# define __MAP_HPP__

#include <world/layer.hpp>
#include <world/position.hpp>

#define LAYER_NUMBER 5
#define LEVEL_HEIGHT 24
static constexpr unsigned int TILE_HEIGHT = 72;
static constexpr unsigned int TILE_WIDTH = 96;
static constexpr unsigned int HALF_TILE_W = TILE_WIDTH / 2;
static constexpr float TILE_HW_RATIO = static_cast<float>(TILE_HEIGHT) / TILE_WIDTH;

// In tiles.
#define TILESET_WIDTH 2
#define TILESET_HEIGHT 6

#define MAPS_DIRECTORY "./data/maps/"

/**
 * Defines the heights of the four corners of various tiles based on where
 * they are positionned in the tile set.  For example a tile positionned on
 * the second line of the tileset has these heights:
 *
 * left=0, top=1, right=1, bottom=0.
 */
static const char tile_heights[6][4] =
{
  {1, 0, 0, 1},
  {0, 1, 1, 0},
  {1, 1, 0, 0},
  {0, 0, 1, 1},
  {1, 1, 1, 1},
  {0, 0, 0, 0},
};

/**
 * The four heights (each corner) of a cell. The special value 7 is used a
 * “no-height” value.
 * Since the maximum number of layers is 4, the maximum height is 5.
 */

union TileHeights
{
  struct
  {
    unsigned left:3;
    unsigned top:3;
    unsigned right:3;
    unsigned bottom:3;
  } corners;
  unsigned int value;
};

typedef std::list<std::size_t> cell_path_t;

using Cell = std::tuple<unsigned short, unsigned short>;
using CellIndex = std::size_t;
static constexpr CellIndex InvalidCellIndex = std::numeric_limits<CellIndex>::max();

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
  TileHeights get_cell_heights(const int cellx, const int celly) const;
  TileHeights get_cell_heights(const CellIndex cell) const;
  /**
   * Returns whether or not this cell can be built on.
   */
  bool can_be_built_on(const int cellx, const int celly) const;
  cell_path_t do_astar(const uint startx, const uint starty,
                                    const uint endx, const uint endy);
  /**
   * Returns the neighbour cells of the given one if it's walkable to it.
   * It's walkable if the two adjacent and corresponding heights are equal.
   * If one of them is different, it's not walkable.  The return vector can
   * contain 2, 3 or 4 values.
   */
  std::vector<std::size_t> get_neighbour_nodes(const std::size_t);

  /**
   * Convert a cell from one representation to the other
   */
  Cell index_to_cell(const CellIndex& index);
  CellIndex cell_to_index(const Cell& cell);

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
public:
  std::vector<Layer*> layers;
protected:
  /**
   * A representation of the map, containing 4 heights for each tile (one
   * for each corner).
   * The heights in each cell is based on the tile_heights value + the level
   * value.
   */
   TileHeights* walking_map;

private:
  Map(const Map&);
  Map& operator=(const Map&);
};

typedef struct
{
  std::size_t index;
  int g;
  int f;
} t_node;

typedef std::vector<std::size_t> t_closed_nodes;
typedef std::list<t_node> t_nodes;

/**
 * Insert a node in the given list, in the correct position as to have the
 * list always ordered by t_score.fvalue in ascendant order.  If it's
 * already present, remove it and reinsert it, updating the associated
 * score.
 */
void insert_node(t_nodes& nodes, std::size_t index, int g, int f);
/**
 * Just returns true if the node is in the set.
 */
bool is_in_set(std::size_t index, const t_closed_nodes& nodes);
/**
 * Returns true if the given index score is lower than the one already in
 * the set. Returns true as well if the index is not in the set already.
 */
bool is_better_than_previously_open(const std::size_t index, const int score, const t_nodes& open);
cell_path_t reconstruct_path(const std::map<std::size_t, std::size_t>& came_from, const std::size_t end);
int heuristic();

#endif // __MAP_HPP__
