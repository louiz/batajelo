#include <gui/menu/menu.hpp>
#include <gui/menu/widget.hpp>

int main()
{
  // Create the main window
  sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(800, 600), "Menu coucou");

  Menu* menu = new Menu(window);

  sf::Clock clock;

  while (window->isOpen())
    {
      sf::Event event;
      while (window->pollEvent(event))
	{
	  if (event.type == sf::Event::Closed)
	    window->close();
	  menu->handle_event(event);
	}
      sf::Time dt = clock.restart();
      menu->update(dt);
      menu->draw();
      window->display();
    }
 
  return EXIT_SUCCESS;
}
