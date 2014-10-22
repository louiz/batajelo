#include <map>
#include <climits>

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
  if (walking_map)
    delete[] walking_map;
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
  BOOST_FOREACH(boost::property_tree::ptree::value_type &property,
                tree.get_child("properties"))
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

uint Map::get_height_in_pixels() const
{
  return this->height;
}

uint Map::get_width_in_pixels() const
{
  return this->width;
}

uint Map::get_height_in_tiles() const
{
  return this->height_in_tiles;
}

uint Map::get_width_in_tiles() const
{
  return this->width_in_tiles;
}

void Map::generate_walking_map()
{
  const uint map_size = this->get_width_in_tiles() * this->get_height_in_tiles();
  this->walking_map = new TileHeights[map_size];
  int level;
  for (uint i = 0; i < map_size; ++i)
    {
      level = this->get_max_level_for_cell(i);
      if (level == -1)
        { // in case of a hole, we put a height of 7 in the four corners.
          // 7 means it's not walkable at all.
          this->walking_map[i].value = ~0;
        }
      else
        {
          if (level != 0)
            {
              log_debug("max level for cell " << i << "=" << level);
            }
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

int Map::get_max_level_for_cell(const uint cell) const
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

TileHeights Map::get_cell_heights(const int col, const int row) const
{
  assert(col >= 0);
  assert(row >= 0);
  assert(col < this->get_width_in_tiles());
  assert(row < this->get_height_in_tiles());
  std::size_t index = (this->get_width_in_tiles() * row) + col;
  return this->walking_map[index];
}

TileHeights Map::get_cell_heights(const CellIndex cell) const
{
  return this->get_cell_heights(cell % this->get_width_in_tiles(),
                                cell / this->get_width_in_tiles());
}

bool Map::can_be_built_on(const int cellx, const int celly) const
{
  // assert(cellx >= 0);
  // assert(celly >= 0);
  // assert(cellx < this->get_width_in_tiles());
  // assert(celly < this->get_height_in_tiles());
  // std::size_t index = (this->get_width_in_tiles() * celly) + cellx;
  // TileHeights heights = this->walking_map[index];
  // ushort a = (heights >> 12) & 15;
  // ushort b = (heights >> 8) & 15;
  // ushort c = (heights >> 4) & 15;
  // ushort d = (heights) & 15;
  // if (a == b && b == c && c == d)
  //   return true;
  return false;
}

cell_path_t Map::do_astar(const uint startx, const uint starty,
                   const uint endx, const uint endy)
{
  log_debug(startx);
  log_debug(starty);
  assert(startx < this->get_width_in_tiles());
  assert(starty < this->get_height_in_tiles());
  assert(endx < this->get_width_in_tiles());
  assert(endy < this->get_height_in_tiles());

  std::size_t start = (this->get_width_in_tiles() * starty) + startx;
  std::size_t goal = (this->get_width_in_tiles() * endy) + endx;
  t_nodes open;
  t_closed_nodes closed;
  insert_node(open, start, 0, 0);
  std::map<std::size_t, std::size_t> came_from;

  t_node current;
  while (open.empty() == false)
    {
      current = open.front();
      if (current.index == goal)
        {
          return reconstruct_path(came_from, current.index);
        }
      open.pop_front();
      closed.push_back(current.index);
      std::vector<std::size_t> neighbours = this->get_neighbour_nodes(current.index);
      for (std::vector<std::size_t>::const_iterator it = neighbours.begin();
           it != neighbours.end(); ++it)
        {
          if (is_in_set((*it), closed) == true)
            {
              continue;
            }
          int tentative_g = current.g + 1;
          if (is_better_than_previously_open((*it), tentative_g, open) == true)
            {
              insert_node(open, (*it), tentative_g, tentative_g);
              came_from[(*it)] = current.index;
            }
          else
            {
              log_error("Is not better than the one already opened");
            }
        }
    }
  cell_path_t ret;
  return ret;
}

std::vector<std::size_t> Map::get_neighbour_nodes(const std::size_t index)
{
  const uint x = index % this->get_width_in_tiles();
  const uint y = index / this->get_width_in_tiles();
  ushort heights = this->get_cell_heights(x, y);
  std::vector<std::size_t> res;
  if (y != 0)
    { // Try top
      std::size_t neighbour = index - this->get_width_in_tiles();
      ushort neighbour_heights = this->get_cell_heights(
                                   neighbour % this->get_width_in_tiles(),
                                   neighbour / this->get_width_in_tiles());
      if ((((heights) & 15) == ((neighbour_heights >> 12) & 15)) &&
          (((heights >> 4) & 15) == ((neighbour_heights >> 8) & 15)))
        res.push_back(neighbour);
    }
  if (x != 0)
    { // Try left
      std::size_t neighbour = index - 1;
      ushort neighbour_heights = this->get_cell_heights(
                                   neighbour % this->get_width_in_tiles(),
                                   neighbour / this->get_width_in_tiles());
      if ((((heights) & 15) == ((neighbour_heights >> 4) & 15)) &&
          (((heights >> 12) & 15) == ((neighbour_heights >> 8) & 15)))
        res.push_back(neighbour);
    }
  if ((x != this->get_width_in_tiles() - 1))
    { // Try right
      std::size_t neighbour = index + 1;
      ushort neighbour_heights = this->get_cell_heights(
                                   neighbour % this->get_width_in_tiles(),
                                   neighbour / this->get_width_in_tiles());
      if ((((heights >> 4) & 15) == ((neighbour_heights) & 15)) &&
          (((heights >> 8) & 15) == ((neighbour_heights >> 12) & 15)))
        res.push_back(neighbour);
    }
  if ((y != this->get_width_in_tiles() - 1))
    { // Try bottom
      std::size_t neighbour = index + this->get_width_in_tiles();
      ushort neighbour_heights = this->get_cell_heights(
                                   neighbour % this->get_width_in_tiles(),
                                   neighbour / this->get_width_in_tiles());
      if ((((heights >> 8) & 15) == ((neighbour_heights >> 4) & 15)) &&
          (((heights >> 12) & 15) == ((neighbour_heights) & 15)))
        res.push_back(neighbour);
    }
  return res;
}

int heuristic()
{
  return 0;
}

void insert_node(t_nodes& nodes, std::size_t index, int g, int f)
{
  std::list<t_node>::iterator it;
  bool inserted = false;
  t_node node;
  node.f = f;
  node.g = g;
  node.index = index;

  for (it = nodes.begin(); it != nodes.end(); ++it)
    {
      // Remove the node from the list if already present.
      if ((*it).index == index)
        {
          it = nodes.erase(it);
        }
    }
  for (it = nodes.begin(); it != nodes.end(); ++it)
    {
      if (((*it).f >= f))
        {
          nodes.insert(it, node);
          return ;
        }
    }
  nodes.insert(nodes.end(), node);
}

bool is_in_set(std::size_t index, const t_closed_nodes& nodes)
{
  std::vector<std::size_t>::const_iterator it;
  for (it = nodes.begin(); it != nodes.end(); ++it)
    {
      if ((*it) == index)
        return true;
    }
  return false;
}

bool is_better_than_previously_open(const std::size_t index, const int score, const t_nodes& open)
{
  std::list<t_node>::const_iterator it;
  for (it = open.begin(); it != open.end(); ++it)
    {
      if ((*it).index == index)
        {
          if ((*it).g < score)
            return false;
          else
            return true;
        }
    }
  return true;
}

cell_path_t reconstruct_path(const std::map<std::size_t, std::size_t>& came_from,
                                          const std::size_t end)
{
  cell_path_t res;
  std::map<std::size_t, std::size_t>::const_iterator it;
  res.push_back(end);
  it = came_from.find(end);
  while (it != came_from.end())
    {
      res.push_back((*it).second);
      it = came_from.find((*it).second);
    }
  return res;
}

CellIndex Map::cell_to_index(const Cell& cell)
{
  return std::get<0>(cell) + (std::get<1>(cell) * this->get_width_in_tiles());
}

Cell Map::index_to_cell(const CellIndex& index)
{
  const unsigned short col = index % this->get_width_in_tiles();
  const unsigned short row = index / this->get_width_in_tiles();
  return std::make_tuple(col, row);
}
