#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/game_client.hpp>
#include <world/client_world.hpp>
#include <world/time.hpp>
#include <gui/screen/screen.hpp>
#include <network/command.hpp>
#include <gui/camera/map.hpp>

int main()
{
  sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1920, 1080),
  						  "Bata");
  window->setMouseCursorVisible(false);
  GameClient* c = new GameClient();
  GraphMap* map = new GraphMap;
  map->load_from_file("test4.tmx");
  ClientWorld* world = new ClientWorld(map);
  world->set_next_turn_callback(boost::bind(&ClientWorld::on_next_turn, world, _1));


  c->install_callback("NEW_OCCUPANT",
          	      boost::bind(&ClientWorld::new_occupant_callback, world, _1));
  c->install_callback("OCCUPANT_LEFT",
          	      boost::bind(&ClientWorld::occupant_left_callback, world, _1));
  c->install_callback("NEW_UNIT",
          	      boost::bind(&ClientWorld::new_unit_callback, world, _1));
  c->install_callback("START",
          	      boost::bind(&ClientWorld::handle_start_command, world, _1));
  c->install_callback("OK",
          	      boost::bind(&ClientWorld::ok_callback, world, _1));
  c->install_callback("T",
          	      boost::bind(&ClientWorld::turn_callback, world, _1));
  c->install_callback("PATH",
          	      boost::bind(&ClientWorld::path_callback, world, _1));
  c->install_callback("BUILD",
          	      boost::bind(&ClientWorld::build_callback, world, _1));

  // c->connect("88.190.23.192", 7879);
  c->connect("127.0.0.1", 7879);

  while (world->is_started() == false)
    {
      // Here be a loading screen.
      c->poll(10);
    }

  Screen screen(world, map, window);

  sf::Clock fps_clock;

  Time time1 = boost::posix_time::microsec_clock::universal_time();
  Time time2;
  Duration dt;

  // window->setMouseCursorVisible(false);
  while (window->isOpen())
    {
      // Check for user events
      sf::Event event;
      while (window->pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
            {
              window->close();
            }
          // if (event.type == sf::Event::GainedFocus)
          //   {
          //   }
          screen.handle_event(event);
        }
      Command* command;
      while ((command = world->get_pending_command()))
        {
          log_debug("PENDING COMMAND!");
          c->send(command);
        }

      c->poll(0);

      // Get the elapsed time
      time2 = boost::posix_time::microsec_clock::universal_time();
      dt += time2 - time1;
      time1 = time2;

      screen.update(dt);
      // Update everything, based on the elapsed time
      long i = get_number_of_updates(dt);
      for (; i > 0; --i)
        {
          world->tick();
        }

      // Draw the result on the screen. Limit to ~60 fps.
      if (fps_clock.getElapsedTime().asMicroseconds() > 10000)
        {
  	  window->clear(sf::Color(70, 80, 80));
          // sf::View view(sf::FloatRect(0, 0, 960, 540));
          // window->setView(view);
  	  screen.draw();
  	  window->display();
  	  fps_clock.restart();
  	}
    }
  delete c;
  delete world;
  delete window;

  return 0;
}

