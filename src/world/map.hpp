#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <map>
#include <list>
#include <tuple>
#include <limits>

#ifndef __MAP_HPP__
# define __MAP_HPP__

#include <world/layer.hpp>
#include <world/position.hpp>
#include <world/path.hpp>

#define LAYER_NUMBER 5
#define LEVEL_HEIGHT 24
static constexpr unsigned int TILE_HEIGHT = 72;
static constexpr unsigned int TILE_WIDTH = 96;
static constexpr unsigned int HALF_TILE_W = TILE_WIDTH / 2;
static constexpr float TILE_HW_RATIO = static_cast<float>(TILE_HEIGHT) / TILE_WIDTH;
static constexpr unsigned int TILE_TOP_OFFSET = 56;

// In tiles.
#define TILESET_WIDTH 2
#define TILESET_HEIGHT 6

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

using Cell = std::tuple<unsigned short, unsigned short>;
using CellIndex = std::size_t;
using CellPath = std::vector<CellIndex>;
static constexpr CellIndex InvalidCellIndex = std::numeric_limits<CellIndex>::max();
static constexpr Cell InvalidCell(std::numeric_limits<unsigned short>::max(),
                                  std::numeric_limits<unsigned short>::max());

class Camera;

class Map
{
  friend class Camera;
public:
  Map();
  virtual ~Map();
  bool load_from_file(const std::string&);
  unsigned int get_height_in_pixels() const;
  unsigned int get_width_in_pixels() const;
  unsigned int get_height_in_tiles() const;
  unsigned int get_width_in_tiles() const;
  int get_max_level_for_cell(const unsigned int cell) const;
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
  TileHeights get_cell_heights(const unsigned int cellx, const unsigned int celly) const;
  TileHeights get_cell_heights(const CellIndex cell) const;
  CellPath do_astar(const Cell start, const Cell end);
  /**
   * Returns the neighbour cells of the given one if it's walkable to it.
   * It's walkable if the two adjacent and corresponding heights are equal.
   * If one of them is different, it's not walkable.  The return vector can
   * contain between 0 and 4 values.
   */
  std::vector<CellIndex> get_neighbour_walkable_cells(const CellIndex);
  /**
   * Returns the neighbour cells that are considered to “partially” hide the
   * given one.
   *
   * A neighbour cell “a” can hide a cell “c” if a has a bigger column
   * (biggest value on the y axis), and one of its adjacent corner from “a”
   * is higher than “c”.
   * The vector may contain between 0 and 3 values.
   */
  bool is_cell_in_neighbour_lower_cells(const CellIndex, const CellIndex);

  /**
   * Convert a cell from one representation to the other
   */
  Cell index_to_cell(const CellIndex& index) const;
  CellIndex cell_to_index(const Cell& cell) const;

protected:
  bool read_layer(boost::property_tree::ptree& tree);
  bool get_layer_level(boost::property_tree::ptree& tree, unsigned int& level);
  bool get_layer_data(boost::property_tree::ptree& tree, std::string& data);
  /**
   * The width (in pixels) of the map.
   */
  unsigned int width;
  /**
   * The height (in pixels) of the map.
   */
  unsigned int height;
  /**
   * The number of tiles, horizontally.
   */
  unsigned int width_in_tiles;
  /**
   * The number of tiles, vertically.
   */
  unsigned int height_in_tiles;
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

struct AStarNode
{
  CellIndex index;
  int g;
  int f;
};

using AStarNodes = std::vector<AStarNode>;

/**
 * Insert a node in the given list, in the correct position as to have the
 * list always ordered by t_score.fvalue in ascendant order.  If it's
 * already present, remove it and reinsert it, updating the associated
 * score.
 */
void insert_node(AStarNodes& nodes, CellIndex index, int g, int f);
/**
 * Just returns true if the node is in the set.
 */
bool is_in_set(CellIndex index, const std::vector<CellIndex>& nodes);
/**
 * Returns true if the given index score is lower than the one already in
 * the set. Returns true as well if the index is not in the set already.
 */
bool is_better_than_previously_open(const CellIndex index, const int score, const AStarNodes& open);
CellPath reconstruct_path(const std::map<std::size_t, std::size_t>& came_from, const std::size_t end);
int heuristic();

std::ostream& operator<<(std::ostream& os, Cell cell);
std::ostream& operator<<(std::ostream& os, TileHeights heights);

#endif // __MAP_HPP__
