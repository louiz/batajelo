#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/game_client.hpp>
#include <game/game.hpp>
#include <world/client_world.hpp>
#include <world/time.hpp>
#include <gui/camera/camera.hpp>
#include <network/command.hpp>

int main()
{
  GameClient* c = new GameClient();
  ClientWorld* world = new ClientWorld;
  world->set_next_turn_callback(boost::bind(&ClientWorld::on_next_turn, world, _1));
  Camera* camera = new Camera(world);

  sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(800, 600),
						  "Bata");

  c->install_callback("NEW_OCCUPANT",
		      boost::bind(&ClientWorld::new_occupant_callback, world, _1));
  c->install_callback("OCCUPANT_LEFT",
		      boost::bind(&ClientWorld::occupant_left_callback, world, _1));
  c->install_callback("NEW_ENTITY",
		      boost::bind(&ClientWorld::new_entity_callback, world, _1));
  c->install_callback("START",
		      boost::bind(&ClientWorld::handle_start_command, world, _1));
  c->install_callback("OK",
		      boost::bind(&ClientWorld::ok_callback, world, _1));
  c->install_callback("T",
		      boost::bind(&ClientWorld::turn_callback, world, _1));
  c->install_callback("PATH",
		      boost::bind(&ClientWorld::path_callback, world, _1));


  // c->connect("88.190.23.192", 7879);
  c->connect("127.0.0.1", 7879);

  sf::Clock fps_clock;

  Time time1 = boost::posix_time::microsec_clock::universal_time();
  Time time2;
  Duration dt;


  while (window->isOpen())
    {
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
	  log_debug("PENDING COMMAND!");
      	  c->send(command);
      	}

      c->poll(10);

      // Get the elapsed time
      time2 = boost::posix_time::microsec_clock::universal_time();
      dt += time2 - time1;
      time1 = time2;

      long i = get_number_of_updates(dt);
      for (; i > 0; --i)
	{
	  world->tick();
	}

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

  return 0;
}
