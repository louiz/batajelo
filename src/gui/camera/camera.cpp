#include <gui/camera/camera.hpp>

Camera::Camera(World* world):
  x(0),
  y(0),
  zoom(1),
  world(world)
{
}

Camera::~Camera()
{
}

void Camera::draw(sf::RenderWindow* win)
{
  Entity* entity;
  while ((entity = this->world->get_next_entity()))
    {
      this->draw_entity(win, entity);
    }
}

void Camera::draw_entity(sf::RenderWindow* win, const Entity* entity)
{
  // TODO check against its position, etc.
  sf::CircleShape circle;
  circle.setRadius(entity->width / 2);
  if (entity->is_selected())
    circle.setOutlineColor(sf::Color::Red);
  else
    circle.setOutlineColor(sf::Color::Blue);
  circle.setOutlineThickness(5);
  circle.setPosition(entity->x, entity->y);
  win->draw(circle);
}

void Camera::handle_event(const sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == 0)
    {
      Entity* entity;
      while ((entity = this->world->get_next_entity()))
	{
	  if (entity->contains(event.mouseButton.x, event.mouseButton.y))
	    entity->selected = true;
	  else
	    entity->selected = false;
	}
    }
  else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == 1)
    {
      
    }
}

void Camera::update(const sf::Time& dt)
{

}
