#include <gui/camera/camera.hpp>
#include <world/world.hpp>

int main()
{
  // Create the main window
  sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(800, 600), "Menu coucou");

  World* world = new World;
  world->load_test();
  Camera* camera = new Camera(world);

  sf::Clock clock;

  while (window->isOpen())
    {
      sf::Event event;
      while (window->pollEvent(event))
	{
	  if (event.type == sf::Event::Closed)
	    window->close();
	  camera->handle_event(event);
	}
      sf::Time dt = clock.restart();
      camera->update(dt);
      window->clear();
      camera->draw(window);
      window->display();
    }
  return EXIT_SUCCESS;
}
