#ifndef ORIENTED_SPRITE_SHEETS_HPP_INCLUDED
#define ORIENTED_SPRITE_SHEETS_HPP_INCLUDED

/**
 * Contains 8 sprite sheets, one for each direction
 */

#include <world/direction.hpp>
#include <gui/sprite_sheet.hpp>

class OrientedSpriteSheets
{
public:
  OrientedSpriteSheets() = default;
  ~OrientedSpriteSheets() = default;

  void load(const std::string& directory, const std::string& prefix,
                       const sf::Vector2<int> size)
  {
    sheets[0].load(directory + prefix + "_E.png", size);
    sheets[1].load(directory + prefix + "_SE.png", size);
    sheets[2].load(directory + prefix + "_S.png", size);
    sheets[3].load(directory + prefix + "_SW.png", size);
    sheets[4].load(directory + prefix + "_W.png", size);
    sheets[5].load(directory + prefix + "_NW.png", size);
    sheets[6].load(directory + prefix + "_N.png", size);
    sheets[7].load(directory + prefix + "_NE.png", size);
  }

  const sf::Texture& get_texture(Direction& direction, const std::size_t index) const
  {
    return this->sheets[static_cast<unsigned short>(direction)].get_texture(index);
  }
  std::size_t get_nb_textures() const
  {
    return this->sheets[0].get_nb_textures();
  }

private:
  std::array<SpriteSheet, static_cast<unsigned short>(Direction::count)> sheets;

  OrientedSpriteSheets(const OrientedSpriteSheets&) = delete;
  OrientedSpriteSheets(OrientedSpriteSheets&&) = delete;
  OrientedSpriteSheets& operator=(const OrientedSpriteSheets&) = delete;
  OrientedSpriteSheets& operator=(OrientedSpriteSheets&&) = delete;
};


#endif /* ORIENTED_SPRITE_SHEETS_HPP_INCLUDED */
