#include <map>
#include <climits>
#include <algorithm>

#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/trim_all.hpp>

#include <logging/logging.hpp>
#include <world/map.hpp>
#include <utils/base64.hpp>
#include <utils/zlib.hpp>

Map::Map():
  width(0),
  height(0),
  width_in_tiles(0),
  height_in_tiles(0),
  walking_map(nullptr)
{
  for (int i = 0; i < LAYER_NUMBER; ++i)
    this->layers.push_back(new Layer);
}

Map::~Map()
{
  std::vector<Layer*>::iterator it;
  for (it = this->layers.begin(); it < this->layers.end(); ++it)
    delete *it;
  if (this->walking_map)
    delete[] this->walking_map;
}

bool Map::load_from_file(const std::string& map_name)
{
  std::string filename(MAPS_DIRECTORY);
  filename += map_name;
  log_debug("Loading map: " << filename);

  boost::property_tree::ptree tree;
  try
    {
      boost::property_tree::read_xml(filename, tree);
    }
  catch (std::runtime_error& e)
    {
      log_error(e.what());
      return false;
    }

  const unsigned int tile_height = tree.get<unsigned int>("map.<xmlattr>.tileheight", 0);
  const unsigned int tile_width = tree.get<unsigned int>("map.<xmlattr>.tilewidth", 0);
  log_debug("Tile size: " << tile_width << ", " << tile_height);
  if ((tile_width != TILE_WIDTH) || (tile_height != TILE_HEIGHT))
    {
      log_error("Map has a wrong tile size: " << tile_width << ":" << tile_height);
      return false;
    }
  this->height_in_tiles = tree.get<unsigned int>
    ("map.<xmlattr>.height", 0);
  this->width_in_tiles = tree.get<unsigned int>
    ("map.<xmlattr>.width", 0);
  BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                tree.get_child("map"))
    {
      if (v.first == "layer")
        {
          if (this->read_layer(v.second) == false)
            return false;
        }
    }
  log_debug("Map size in px " << this->width << ", " << this->height);
  log_debug("Map size, in tiles: " << this->width_in_tiles << ", " << this->height_in_tiles);
  this->generate_walking_map();
  return true;
}

bool Map::read_layer(boost::property_tree::ptree& tree)
{
  const unsigned int layer_height = tree.get<unsigned int>
    ("<xmlattr>.height", 0);
  const unsigned int layer_width = tree.get<unsigned int>
    ("<xmlattr>.width", 0);
  if (layer_width == 0 or layer_width == 0)
    {
      log_error("Layer has an invalid size: " <<
                layer_height << ":" <<
                layer_width);
      return false;
    }
  unsigned int level;
  if ((this->get_layer_level(tree, level) == false))
    return false;
  log_debug("Reading layer for level " << level);
  Layer* layer = this->layers[level];
  layer->set_level(level);
  layer->set_size(layer_width, layer_height);

  if ((layer_width * TILE_WIDTH) >= this->width)
    {
      this->width = layer_width * TILE_WIDTH;
      this->width_in_tiles = layer_width;
    }
  if ((layer_height * TILE_HEIGHT) >= this->height)
    {
      // Divide by two because of the tile shifting each two rows
      this->height = layer_height * TILE_HEIGHT / 2;
      this->height_in_tiles = layer_height;
    }
  std::string data;
  if (this->get_layer_data(tree, data) == false)
    return false;
  const int size = layer_width * layer_height * 4;

  for (int i = 0; i < size - 3; i += 4)
    {
      const unsigned int gid = data[i] |
        data[i + 1] << 8 |
        data[i + 2] << 16 |
        data[i + 3] << 24;
      layer->set_cell(i/4, gid);
    }
  return true;
}

bool Map::get_layer_level(boost::property_tree::ptree& tree, unsigned int& level)
{
  if (!tree.get_child_optional("properties"))
    {
      log_error("No properties for the layer");
      return false;
    }
  for (const boost::property_tree::ptree::value_type &property: tree.get_child("properties"))
    if ((property.first == "property") &&
        (property.second.get<std::string>("<xmlattr>.name", "") == "level"))
      {
        try
          {
            level = property.second.get<unsigned int>("<xmlattr>.value");
          }
        catch (std::exception& e)
          {
            log_error("Layer level is invalid: " << e.what());
            return false;
          }
        if (level >= this->layers.size())
          {
            log_error("Layer level is too big: " << level);
            return false;
          }
        else
          return true;
      }
  log_debug("Level property not found");
  return false;
}

bool Map::get_layer_data(boost::property_tree::ptree& tree, std::string& data)
{
  BOOST_FOREACH(boost::property_tree::ptree::value_type &child, tree)
    {
      if (child.first == "data")
        {
          boost::property_tree::ptree node = child.second;
          if ((node.get<std::string>("<xmlattr>.encoding", "") != "base64") ||
              (node.get<std::string>("<xmlattr>.compression", "") != "zlib"))
            {
              log_error("Wrong data encoding/compression");
              return false;
            }
          std::string content(node.get<std::string>("", ""));
          boost::algorithm::trim_all(content);
            try
              {
                data = zlib_decompress(base64_decode(content));
              }
            catch (std::exception& e)
              {
                log_error(e.what());
                return false;
              }
          return true;
        }
    }
  log_error("No data found for layer");
  return false;
}

unsigned int Map::get_height_in_pixels() const
{
  return this->height;
}

unsigned int Map::get_width_in_pixels() const
{
  return this->width;
}

unsigned int Map::get_height_in_tiles() const
{
  return this->height_in_tiles;
}

unsigned int Map::get_width_in_tiles() const
{
  return this->width_in_tiles;
}

void Map::generate_walking_map()
{
  const unsigned int map_size = this->get_width_in_tiles() * this->get_height_in_tiles();
  this->walking_map = new TileHeights[map_size];
  int level;
  for (unsigned int i = 0; i < map_size; ++i)
    {
      level = this->get_max_level_for_cell(i);
      if (level == -1)
        { // in case of a hole, we put a height of 7 in the four corners.
          // 7 means it's not walkable at all.
          this->walking_map[i].value = ~0;
        }
      else
        {
          const unsigned int tile_value = this->layers[level]->get_cell(i);
          if (tile_value == 0)
            this->walking_map[i].value = ~0;
          else
            {
              const char* tile = tile_heights[(this->layers[level]->get_cell(i) - 1) / TILESET_WIDTH];

              this->walking_map[i].corners.left    = tile[0] + level;
              this->walking_map[i].corners.top     = tile[1] + level;
              this->walking_map[i].corners.right   = tile[2] + level;
              this->walking_map[i].corners.bottom  = tile[3] + level;
            }
        }
    }
}

int Map::get_max_level_for_cell(const unsigned int cell) const
{
  std::vector<Layer*>::const_iterator it;
  int ret = -1;
  for (it = this->layers.begin(); it < this->layers.end(); ++it)
    {
      if ((*it)->get_cell(cell) != 0)
        ret = (*it)->get_level();
    }
   return ret;
}

TileHeights Map::get_cell_heights(const unsigned int col, const unsigned int row) const
{
  std::size_t index = (this->get_width_in_tiles() * row) + col;
  return this->walking_map[index];
}

TileHeights Map::get_cell_heights(const CellIndex cell) const
{
  return this->get_cell_heights(cell % this->get_width_in_tiles(),
                                cell / this->get_width_in_tiles());
}

CellPath Map::do_astar(const Cell start, const Cell goal)
{
  unsigned short startx;
  unsigned short starty;
  unsigned short endx;
  unsigned short endy;

  std::tie(startx, starty) = start;
  std::tie(endx, endy) = goal;

  log_debug("do_astar");
  log_debug(startx << " " << starty);
  log_debug(endx << " " << endy);

  CellIndex end = this->cell_to_index(goal);
  if (end == InvalidCellIndex)
    return {};

  assert(startx < this->get_width_in_tiles());
  assert(starty < this->get_height_in_tiles());
  assert(endx < this->get_width_in_tiles());
  assert(endy < this->get_height_in_tiles());

  AStarNodes open;
  std::vector<CellIndex> closed;
  insert_node(open, this->cell_to_index(start), 0, 0);
  std::map<CellIndex, CellIndex> came_from;

  AStarNode current;
  while (open.empty() == false)
    {
      current = open.front();
      if (current.index == end)
        return reconstruct_path(came_from, current.index);
      open.erase(open.begin());
      closed.push_back(current.index);
      auto neighbours = this->get_neighbour_walkable_cells(current.index);
      for (const CellIndex& neighbour: neighbours)
        {
          if (is_in_set(neighbour, closed) == true)
            continue;
          int tentative_g = current.g + 1;
          if (is_better_than_previously_open(neighbour, tentative_g, open) == true)
            {
              insert_node(open, neighbour, tentative_g, tentative_g);
              came_from[neighbour] = current.index;
            }
        }
    }
  return {};
}

std::vector<CellIndex> Map::get_neighbour_walkable_cells(const CellIndex index)
{
  const Cell cell = this->index_to_cell(index);

  const TileHeights heights = this->get_cell_heights(index);

  const unsigned short x = std::get<0>(cell);
  const unsigned short y = std::get<1>(cell);

  std::vector<CellIndex> res;
  res.reserve(4);
  if (y != 0 && x != 0)
    { // Try top left
      CellIndex neighbour;
      if (y % 2 == 0)
        neighbour = this->cell_to_index(std::make_tuple(x - 1, y - 1));
      else
        neighbour = this->cell_to_index(std::make_tuple(x, y - 1));
      const TileHeights neighbour_heights = this->get_cell_heights(neighbour);

      if (heights.corners.left == neighbour_heights.corners.bottom &&
          heights.corners.top == neighbour_heights.corners.right)
        res.push_back(neighbour);
    }
  if (y != 0 && x != this->get_width_in_tiles() - 1)
    { // Try top right
      CellIndex neighbour;
      if (y % 2 == 0)
        neighbour = this->cell_to_index(std::make_tuple(x, y - 1));
      else
        neighbour = this->cell_to_index(std::make_tuple(x + 1, y - 1));

      const TileHeights neighbour_heights = this->get_cell_heights(neighbour);
      if (heights.corners.right == neighbour_heights.corners.bottom &&
          heights.corners.top == neighbour_heights.corners.left)
        res.push_back(neighbour);
    }
  if (x != this->get_width_in_tiles() - 1 && y != this->get_height_in_tiles() - 1)
    { // Try bottom right
      CellIndex neighbour;
      if (y % 2 == 0)
        neighbour = this->cell_to_index(std::make_tuple(x, y + 1));
      else
        neighbour = this->cell_to_index(std::make_tuple(x + 1, y + 1));

      const TileHeights neighbour_heights = this->get_cell_heights(neighbour);
      if (heights.corners.right == neighbour_heights.corners.top &&
          heights.corners.bottom == neighbour_heights.corners.left)
        res.push_back(neighbour);
    }
  if (x != 0 && y != this->get_width_in_tiles() - 1)
    { // Try bottom left
      CellIndex neighbour;
      if (y % 2 == 0)
        neighbour = this->cell_to_index(std::make_tuple(x - 1, y + 1));
      else
        neighbour = this->cell_to_index(std::make_tuple(x, y + 1));

      const TileHeights neighbour_heights = this->get_cell_heights(neighbour);
      if (heights.corners.left == neighbour_heights.corners.top &&
          heights.corners.bottom == neighbour_heights.corners.right)
        res.push_back(neighbour);
    }
  return res;
}

bool Map::is_cell_in_neighbour_lower_cells(const CellIndex index, const CellIndex near)
{
  const Cell cell = this->index_to_cell(index);

  const TileHeights heights = this->get_cell_heights(index);

  const unsigned short x = std::get<0>(cell);
  const unsigned short y = std::get<1>(cell);

  if (x != this->get_width_in_tiles() - 1 && y != this->get_height_in_tiles() - 1)
    { // Bottom right
      CellIndex neighbour;
      if (y % 2 == 0)
        neighbour = this->cell_to_index(std::make_tuple(x, y + 1));
      else
        neighbour = this->cell_to_index(std::make_tuple(x + 1, y + 1));

      if (neighbour == near)
        {
          const TileHeights neighbour_heights = this->get_cell_heights(neighbour);
          if (heights.corners.right >= neighbour_heights.corners.top &&
              heights.corners.bottom >= neighbour_heights.corners.left)
            return true;
        }
    }
  if (x != 0 && y != this->get_width_in_tiles() - 1)
    { // Bottom left
      CellIndex neighbour;
      if (y % 2 == 0)
        neighbour = this->cell_to_index(std::make_tuple(x - 1, y + 1));
      else
        neighbour = this->cell_to_index(std::make_tuple(x, y + 1));

      if (neighbour == near)
        {
          const TileHeights neighbour_heights = this->get_cell_heights(neighbour);
          if (heights.corners.left >= neighbour_heights.corners.top &&
              heights.corners.bottom >= neighbour_heights.corners.right)
            return true;
        }
    }
  if (// res.size() == 2 && 
      y != this->get_height_in_tiles() - 2)
    { // Bottom
      CellIndex neighbour = this->cell_to_index(std::make_tuple(x, y + 2));
      if (neighbour == near)
        {
          const TileHeights neighbour_heights = this->get_cell_heights(neighbour);
          if (heights.corners.bottom >= neighbour_heights.corners.top)
            return true;
        }
    }
  if (x != this->get_width_in_tiles() - 1)
    {                           // Right
      CellIndex neighbour = this->cell_to_index(std::make_tuple(x + 1, y));
      if (neighbour == near)
        {
          const TileHeights neighbour_heights = this->get_cell_heights(neighbour);
          if (heights.corners.right >= neighbour_heights.corners.left)
            return true;
        }
    }
  if (x != 0)
    {                           // Left
      CellIndex neighbour = this->cell_to_index(std::make_tuple(x - 1, y));
      if (neighbour == near)
        {
          const TileHeights neighbour_heights = this->get_cell_heights(neighbour);
          if (heights.corners.left >= neighbour_heights.corners.right)
            return true;
        }
    }

  return false;
}

int heuristic()
{
  return 0;
}

void insert_node(AStarNodes& nodes, CellIndex index, int g, int f)
{
  AStarNode node;
  node.f = f;
  node.g = g;
  node.index = index;

  // Remove the node from the list if already present.
  nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
                            [index](const auto& node)
                            {
                              return node.index == index;
                            }),
             nodes.end());

  for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
      if ((it->f >= f))
        {
          nodes.insert(it, node);
          return ;
        }
    }

  nodes.insert(nodes.end(), node);
}

bool is_in_set(std::size_t index, const std::vector<CellIndex>& nodes)
{
  return std::find(nodes.begin(), nodes.end(), index) != nodes.end();
}

bool is_better_than_previously_open(const CellIndex index, const int score, const AStarNodes& open)
{
  for (const AStarNode& node: open)
    {
      if (node.index == index)
        {
          if (node.g < score)
            return false;
          else
            return true;
        }
    }
  return true;
}

CellPath reconstruct_path(const std::map<std::size_t, std::size_t>& came_from,
                          const std::size_t end)
{
  CellPath res;
  std::map<std::size_t, std::size_t>::const_iterator it;
  res.push_back(end);
  it = came_from.find(end);
  while (it != came_from.end())
    {
      auto next = came_from.find((*it).second);
      if (next != came_from.end())
        res.push_back((*it).second);
      it = next;
    }
  return res;
}

CellIndex Map::cell_to_index(const Cell& cell) const
{
  if (cell == InvalidCell)
    return InvalidCellIndex;
  return std::get<0>(cell) + (std::get<1>(cell) * this->get_width_in_tiles());
}

Cell Map::index_to_cell(const CellIndex& index) const
{
  if (index == InvalidCellIndex)
    return InvalidCell;
  const unsigned short col = index % this->get_width_in_tiles();
  const unsigned short row = index / this->get_width_in_tiles();
  return std::make_tuple(col, row);
}

std::ostream& operator<<(std::ostream& os, Cell cell)
{
  return os << "Cell(" << std::get<0>(cell) << ":" << std::get<1>(cell) << ")";
}

std::ostream& operator<<(std::ostream& os, TileHeights heights)
{
  return os << "Heights(left: " << heights.corners.left <<
    ", top: " << heights.corners.top <<
    ", right: " << heights.corners.right <<
    ", bottom: " << heights.corners.bottom << ")";
}
