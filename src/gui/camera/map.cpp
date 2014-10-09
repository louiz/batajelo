#include <logging/logging.hpp>
#include <gui/camera/map.hpp>
#include <utils/string.hpp>
#include <boost/utility.hpp>

GraphMap::GraphMap()
{
  // A value of 0 in a cell means no tile, so the TileGraph
  // with gid 0 is NULL;
  this->tiles.push_back(nullptr);
}

GraphMap::~GraphMap()
{
  std::vector<GraphTile*>::iterator it;
  for (it = this->tiles.begin(); it < this->tiles.end(); ++it)
    {
      if ((*it) != nullptr)
        delete *it;
    }
  std::vector<sf::Texture*>::iterator itt;
  for (itt = this->tileset_textures.begin(); itt < this->tileset_textures.end(); ++itt)
    {
      delete *itt;
    }
}

bool GraphMap::load_from_file(const std::string& map_name, bool load_minimap)
{
  // TODO, don't duplicate parsing.
  Map::load_from_file(map_name);
  std::string filename(MAPS_DIRECTORY);
  filename += map_name;

  if (load_minimap)
    {
      std::string minimap_filename(MINIMAPS_DIRECTORY);
      minimap_filename += map_name;
      if (this->minimap_texture.loadFromFile(utils::basename(minimap_filename) + ".png") == false)
        {
          log_error("Could not load minimap image " << minimap_filename);
          return false;
        }
    }
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
  BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                tree.get_child("map"))
    {
      if (v.first == "tileset")
        {
          if (this->read_tileset(v.second) == false)
            return false;
        }
    }
  return true;
}

bool GraphMap::read_tileset(boost::property_tree::ptree& tileset_tree)
{
  if (!tileset_tree.get_child_optional("image"))
    {
      log_error("Image node not found for tileset");
      return false;
    }
  const boost::property_tree::ptree image_tree = tileset_tree.get_child("image");
  const std::string maps_directory(MAPS_DIRECTORY);
  const std::string source = maps_directory + image_tree.get<std::string>("<xmlattr>.source", "");
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
  log_error("Image: " << image_width << ":" << image_height);
  sf::Texture* texture = new sf::Texture;
  if (texture->loadFromFile(source ,
                            sf::Rect<int>(0, 0,
                                          image_width,
                                          image_height)) == false)
    {
      log_error("Could not load image file: " << source);
      delete texture;
      return false;
    }
  else
    log_info("Successfuly loaded image: " << source);
  this->tileset_textures.push_back(texture);
  const uint number_of_tiles = (image_height / tile_height) *
    (image_width / tile_width);
  if (this->tiles.size() < number_of_tiles + gid)
    {
      log_debug("Resizing tiles to " << number_of_tiles + gid);
      this->tiles.resize(number_of_tiles + gid, nullptr);
    }
  for (uint y = 0; y < image_height; y += tile_height)
    for (uint x = 0; x < image_width; x += tile_width)
      {
        log_debug("Cropping tile: " << x << ":" << y << ", " << tile_width << ":" << tile_height << "in " << gid);
        GraphTile* tile = new GraphTile(*texture,
                                        sf::Rect<int>(x, y,
                                                      tile_width,
                                                      tile_height));
        this->tiles[gid++] = tile;
      }
  return true;
}

void GraphMap::draw_full_map(sf::RenderTarget& target)
{
  GraphTile* tile;
  uint level = 0;

  for (auto& layer: this->layers)
    {
      if (!layer->cells)
        continue ;
      uint yoffset = level++ * LEVEL_HEIGHT;
      for (uint y = 0;
           y < this->height_in_tiles;
           y++)
        {
          for (uint x = 0;
               x < this->width_in_tiles;
               x++)
            {
              const uint gid = layer->cells[this->width * y + x];
              tile = this->tiles[gid];
              if (tile != 0)
                {
                  tile->sprite.setPosition(x * TILE_WIDTH,
                                           -64 + y * TILE_HEIGHT - yoffset);
                  target.draw(tile->sprite);
                }
            }
        }
    }
}
