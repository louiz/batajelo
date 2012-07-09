#include <gui/camera/map.hpp>
#include <iostream>

int main(int ac, char** av)
{
  if (ac != 3)
    {
      std::cout << "Generate a minimap image from a map file" << std::endl;
      std::cout << "Usage: " << av[0] << " input.map output.png" << std::endl;
      return 1;
    }
  sf::RenderWindow window(sf::VideoMode(250, 250), "coucou");
  window.setVisible(false);

  GraphMap map;
  if (map.load_from_file(av[1], false) == false)
    {
      std::cerr << "Error: Map could not be parsed correctly, exiting." << std::endl;
      return 1;
    }
  window.clear(sf::Color::Transparent);
  sf::View view(sf::FloatRect(0,
                              0, map.get_width_in_pixels(),
                              map.get_height_in_pixels()));
  window.setView(view);
  map.draw_full_map(window);

  std::cout << "Saving file to " << av[2] << "." << std::endl;
  window.capture().saveToFile(av[2]);
  return 0;
}
