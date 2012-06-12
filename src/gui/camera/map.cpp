#include <gui/camera/map.hpp>
#include <boost/utility.hpp>
#include <utils/base64.hpp>
#include <utils/zlib.hpp>
#include <boost/algorithm/string/trim_all.hpp>

GraphMap::GraphMap()
{
  for (int i = 0; i < LAYER_NUMBER; ++i)
    this->layers.push_back(new Layer);
  // A value of 0 in a cell means no tile, so the TileGraph
  // with gid 0 is NULL;
  this->tiles.push_back(0);
}

GraphMap::~GraphMap()
{
  std::vector<Layer*>::iterator it;
  for (it = this->layers.begin(); it < this->layers.end(); ++it)
    delete *it;
  std::vector<GraphTile*>::iterator itt;
  for (itt = this->tiles.begin(); itt < this->tiles.end(); ++itt)
    {
      if ((*itt) != 0)
        delete *itt;
    }
  std::vector<sf::Texture*>::iterator ittt;
  for (ittt = this->tileset_textures.begin(); ittt < this->tileset_textures.end(); ++ittt)
    {
      delete *ittt;
    }

}

bool GraphMap::load_from_file(const std::string& filename)
{
  // TODO, don't duplicate parsing.
  Map::load_from_file(filename);

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
      else if (v.first == "tileset")
        {
          log_debug(v.second.get("<xmlattr>.name", ""));
          if (this->read_tileset(v.second) == false)
            return false;
        }
    }
  return true;
}

bool GraphMap::read_layer(boost::property_tree::ptree& tree)
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
      if (gid != 0)
        log_debug(i << ":" << gid);
      layer->set_cell(i/4, gid);
    }
  return true;
}

bool GraphMap::read_tileset(boost::property_tree::ptree& tileset_tree)
{
  log_debug("coucou");
  if (!tileset_tree.get_child_optional("image"))
    {
      log_error("Image node not found for tileset");
      return false;
    }
  const boost::property_tree::ptree image_tree = tileset_tree.get_child("image");
  const std::string source = image_tree.get<std::string>("<xmlattr>.source", "");
  const uint image_height = image_tree.get<uint>("<xmlattr>.height", 0);
  const uint image_width = image_tree.get<uint>("<xmlattr>.width", 0);
  const uint tile_height = tileset_tree.get<uint>("<xmlattr>.tileheight", 0);
  const uint tile_width = tileset_tree.get<uint>("<xmlattr>.tilewidth", 0);
  uint gid = tileset_tree.get<uint>("<xmlattr>.firstgid", 0);
  if (source == "")
    {
      log_error("No source file specificied for tileset");
      return false;
    }
  if ((image_width == 0) || (image_height == 0))
    {
      log_error("Wrong tileset image dimensions");
      return false;
    }
  if ((tile_width == 0) || (tile_height == 0))
    {
      log_error("Wrong tile dimensions");
      return false;
    }
  if (gid == 0 || gid > 4096)
    // since we malloc the vector of that size, we could DoS the server
    // by just sending a map with a very big firstgid element. Avoid that.
    {
      log_error("Firstgid for the tileset is invalid.");
      return false;
    }
  sf::Texture* texture = new sf::Texture;
  if (texture->loadFromFile(source ,
                            sf::Rect<int>(0, 0,
                                          image_height,
                                          image_width)) == false)
    {
      log_error("Could not load image file: " << source);
      delete texture;
      return false;
    }
  this->tileset_textures.push_back(texture);
  const uint number_of_tiles = (image_height / tile_height) *
    (image_width / tile_width);
  log_debug("The image will contain " << number_of_tiles << " tiles, and starts with gid number " << gid);
  if (this->tiles.size() < number_of_tiles + gid)
    this->tiles.resize(number_of_tiles + gid, 0);
  log_debug("The tiles list size is now " << this->tiles.size());
  for (uint y = 0; y < image_height; y += tile_height)
    for (uint x = 0; x < image_width; x += tile_width)
      {
        log_debug(x << ":" << y << " = " << gid);
        GraphTile* tile = new GraphTile(*texture,
                                        sf::Rect<int>(x, y,
                                                      tile_width,
                                                      tile_height));
        this->tiles[gid++] = tile;
      }
  return true;
}

bool GraphMap::get_layer_level(boost::property_tree::ptree& tree, unsigned int& level)
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

bool GraphMap::get_layer_data(boost::property_tree::ptree& tree, std::string& data)
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
          log_debug(content)
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

void GraphMap::reset_layers_iterator()
{
  this->layers_iterator = this->layers.begin();
}

Layer* GraphMap::get_next_layer()
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
