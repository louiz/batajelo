#ifndef FOG_HPP_INCLUDED
#define FOG_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Camera;
class World;

/**
 * A texture where we draw the fog of war. We then draw it over the camera
 * display.
 *
 * TODO: optimization, redraw the texture only every n seconds instead of
 * each frame. And invalide it (force a redraw) if the camera moved too
 * much, etc.
 *
 * TODO: Check if the RenderTexture is not supported by the graphic card,
 * and use something else in that case (direct draw on the windowrenderer
 * for example)
 */

class Fog
{
public:
  Fog(const unsigned int width, const unsigned int height,
      const Camera* const camera, const World* const world);
  ~Fog() = default;

  void resize(const unsigned int width, const unsigned int height);

  void redraw();
  void invalidate();
  const sf::Sprite& get_sprite();

private:
  /**
   * The texture to which we draw the fog
   */
  sf::RenderTexture texture;
  /**
   * A sprite to return, using our texture, to be drawn on the screen.
   */
  sf::Sprite sprite;
  /**
   * Whether or not we need to redraw our texture before displaying it.
   */
  bool needs_redraw;

  /**
   * We need a pointer to the camera to know what part of the world's fog
   * needs to be drawn
   */
  const Camera* const camera;
  /**
   * We also need the world, to know the entity position, their vision range
   * etc
   */
  const World* const world;

  Fog(const Fog&) = delete;
  Fog(Fog&&) = delete;
  Fog& operator=(const Fog&) = delete;
  Fog& operator=(Fog&&) = delete;
};


#endif /* FOG_HPP_INCLUDED */
