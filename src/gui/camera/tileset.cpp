#include <boost/property_tree/xml_parser.hpp>

#include <gui/camera/tileset.hpp>
#include <logging/logging.hpp>
#include <utils/string.hpp>
#include <boost/utility.hpp>

Tileset::Tileset()
{
  // A value of 0 in a cell means no tile, so the TileGraph
  // with gid 0 is NULL;
  this->tiles.push_back(nullptr);
}

Tileset::~Tileset()
{
}

bool Tileset::load_from_file(const std::string& map_name)
{
  std::string filename = MAPS_DIRECTORY + map_name;

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

  for (const auto& v: tree.get_child("map"))
    {
      if (v.first == "tileset")
        {
          if (this->read_tileset(v.second) == false)
            return false;
        }
    }
  return true;
}

bool Tileset::read_tileset(const boost::property_tree::ptree& tileset_tree)
{
  if (!tileset_tree.get_child_optional("image"))
    {
      log_error("Image node not found for tileset");
      return false;
    }
  const boost::property_tree::ptree image_tree = tileset_tree.get_child("image");
  const std::string source = MAPS_DIRECTORY + image_tree.get<std::string>("<xmlattr>.source", "");
  const uint32_t image_height = image_tree.get<uint32_t>("<xmlattr>.height", 0);
  const uint32_t image_width = image_tree.get<uint32_t>("<xmlattr>.width", 0);
  const uint32_t tile_height = tileset_tree.get<uint32_t>("<xmlattr>.tileheight", 0);
  const uint32_t tile_width = tileset_tree.get<uint32_t>("<xmlattr>.tilewidth", 0);
  std::size_t gid = tileset_tree.get<uint>("<xmlattr>.firstgid", 0);
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
      log_error("Firstgid for the tileset is invalid: " << gid);
      return false;
    }
  log_error("Image: " << image_width << ":" << image_height);
  auto texture = std::make_unique<sf::Texture>();
  if (texture->loadFromFile(source ,
                            sf::Rect<int>(0, 0,
                                          image_width,
                                          image_height)) == false)
    {
      log_error("Could not load image file: " << source);
      return false;
    }
  else
    log_info("Successfuly loaded image: " << source);
  const uint number_of_tiles = (image_height / tile_height) *
    (image_width / tile_width);
  if (this->tiles.size() < number_of_tiles + gid)
    {
      log_debug("Resizing tiles to " << number_of_tiles + gid);
      this->tiles.resize(number_of_tiles + gid);
    }
  for (uint y = 0; y < image_height; y += tile_height)
    for (uint x = 0; x < image_width; x += tile_width)
      {
        log_debug("Cropping tile: " << x << ":" << y << ", " << tile_width << ":" << tile_height << "in " << gid);
        this->tiles[gid++] = std::make_unique<GraphTile>(*texture,
                                                         sf::Rect<int>(x, y,
                                                                       tile_width,
                                                                       tile_height));
      }
  this->tileset_textures.push_back(std::move(texture));
  return true;
}
