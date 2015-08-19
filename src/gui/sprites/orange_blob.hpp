#ifndef ORANGE_BLOB_HPP_INCLUDED
#define ORANGE_BLOB_HPP_INCLUDED

#include <gui/sprites/entity_sprite.hpp>

#include <gui/oriented_sprite_sheets.hpp>
#include <gui/sprite_sheet.hpp>
#include <gui/animation.hpp>
#include <utils/time.hpp>

class OrangeBlob: public EntitySprite
{
  friend class Camera;
public:
  OrangeBlob(const Entity* const);
  ~OrangeBlob() = default;

  void draw(sf::RenderTarget& surface, const sf::RenderStates& states) const override final;
  void tick() override final {}
  void update(const utils::Duration& dt) override final;
  void on_task_changed(const Task* task) override final;

  static void init();

private:
  static OrientedSpriteSheets idle_sheets;
  static OrientedSpriteSheets walk_sheets;

  Animation idle_animation;
  Animation walk_animation;

  Animation* current_animation;

  OrangeBlob(const OrangeBlob&) = delete;
  OrangeBlob(OrangeBlob&&) = delete;
  OrangeBlob& operator=(const OrangeBlob&) = delete;
  OrangeBlob& operator=(OrangeBlob&&) = delete;
};


#endif /* ORANGE_BLOB_HPP_INCLUDED */
