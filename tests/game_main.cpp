#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/game_client.hpp>
#include <game/game.hpp>
#include <world/world.hpp>
#include <gui/camera/camera.hpp>
#include <game/turn_handler.hpp>
#include <network/command.hpp>

int main()
{
  GameClient* c = new GameClient();
  World* world = new World;
  Camera* camera = new Camera(world);
  TurnHandler* turn_handler = new TurnHandler;

  sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(800, 600),
						  "Bata");

  c->install_callback("NEW_OCCUPANT",
		      boost::bind(&World::new_occupant_callback, world, _1));
  c->install_callback("OCCUPANT_LEFT",
		      boost::bind(&World::occupant_left_callback, world, _1));
  // c->

  // c->connect("88.190.23.192", 7879);
  c->connect("127.0.0.1", 7879);

  sf::Clock clock;
  sf::Clock fps_clock;

  sf::Time dt;

  while (window->isOpen())
    {
      // Network poll
      c->poll();

      // Check for user events
      sf::Event event;
      while (window->pollEvent(event))
      	{
      	  if (event.type == sf::Event::Closed)
      	    window->close();
      	  camera->handle_event(event);
      	}
      Command* command;
      while ((command = world->get_pending_command()))
      	{
      	  c->send(command);
      	}

      // twice?
      c->poll();

      // Get the elapsed time
      dt = clock.restart();

      // // Update everything, based on the elapsed time
      camera->update(dt);

      // Draw the result on the screen. Limit to ~60 fps.
      if (fps_clock.getElapsedTime().asMicroseconds() > 16666)
      	{
	  window->clear();
	  camera->draw(window);
	  window->display();
	  fps_clock.restart();
	}
    }
  delete c;
  delete world;
  delete camera;
  delete window;
  delete turn_handler;

  return 0;
}
