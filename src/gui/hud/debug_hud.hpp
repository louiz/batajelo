#ifndef DEBUG_HUD_HPP
#define DEBUG_HUD_HPP

#include <SFML/Graphics.hpp>

#include <gui/screen/screen_element.hpp>

#include <string>
#include <list>

class GameClient;

class DebugHud: public ScreenElement
{
public:
  DebugHud(GameClient* game_client, Screen* screen);
  ~DebugHud();
  void add_debug_line(const std::string& line, const sf::Color& color=sf::Color::Red);

  void draw() override final;
  void update(const utils::Duration& dt) override final;
  bool handle_event(const sf::Event& event) override final;
private:
  std::list<sf::Text> text_lines;
  sf::Font font;
  GameClient* game_client;

  DebugHud(const DebugHud&) = delete;
  DebugHud(DebugHud&&) = delete;
  DebugHud& operator=(const DebugHud&) = delete;
  DebugHud& operator=(DebugHud&&) = delete;
};


#endif /* DEBUG_HUD_HPP */
