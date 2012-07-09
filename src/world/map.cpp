#include <world/map.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <utils/base64.hpp>
#include <utils/zlib.hpp>
#include <boost/utility.hpp>
#include <boost/foreach.hpp>

Map::Map():
  width(0),
  height(0),
  width_in_tiles(0),
  height_in_tiles(0),
  walking_map(0)
{
  for (int i = 0; i < LAYER_NUMBER; ++i)
    this->layers.push_back(new Layer);
}

Map::~Map()
{
  std::vector<Layer*>::iterator it;
  for (it = this->layers.begin(); it < this->layers.end(); ++it)
    delete *it;
  if (walking_map != 0)
    delete[] walking_map;
}

bool Map::load_from_file(const std::string& map_name)
{
  std::string filename(MAPS_DIRECTORY);
  filename += map_name;

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
  if ((tile_width != TILE_WIDTH) || (tile_height != TILE_HEIGHT))
    {
      log_error("Map has a wrong tile size: " << tile_width << ":" << tile_height);
      return false;
    }
  BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                tree.get_child("map"))
    {
      if (v.first == "layer")
        {
          if (this->read_layer(v.second) == false)
            return false;
        }
    }
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
  log_error(layer_width);
  if ((layer_width * TILE_WIDTH) >= this->width)
    {
      this->width = layer_width * TILE_WIDTH;
      this->width_in_tiles = layer_width;
    }
  if ((layer_height * TILE_HEIGHT) >= this->height)
    {
      this->height = layer_height * TILE_HEIGHT;
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

void Map::reset_layers_iterator()
{
  this->layers_iterator = this->layers.begin();
}

Layer* Map::get_next_layer()
{
  if (this->layers_iterator == this->layers.end())
    {
      this->layers_iterator = this->layers.begin();
      return 0;
    }
  Layer* layer = *this->layers_iterator;
  ++this->layers_iterator;
  return layer;
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
  this->walking_map = new ushort[map_size];
  int level;
  for (uint i = 0; i < map_size; ++i)
    {
      level = this->get_max_level_for_cell(i);
      if (level == -1)
        { // in case of a hole, we put a height of 15 in the four corners.
          // 15 means it's not walkable at all.
          this->walking_map[i] = ~0;
        }
      else
        {
          const char* tile = tile_heights[this->layers[level]->get_cell(i) / TILESET_WIDTH];
          this->walking_map[i] = 0;
          for (uint y = 0; y < 4; ++y)
            {
              assert((tile[y] + level) < 16);
              this->walking_map[i] |= (tile[y] + level) << (y * 4);
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

ushort Map::get_cell_heights(const int cellx, const int celly) const
{
  assert(cellx >= 0);
  assert(celly >= 0);
  assert(cellx < this->get_width_in_tiles());
  assert(celly < this->get_height_in_tiles());
  std::size_t index = (this->get_width_in_tiles() * celly) + cellx;
  return this->walking_map[index];
}
