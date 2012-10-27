#include <config/config.hpp>
#include <gui/ui.hpp>
#include <gui/page/settings.hpp>
#include <gui/page/login.hpp>
#include <gui/page/login_steps.hpp>
#include <gui/popup/popup.hpp>
#include <gui/popup/error_popup.hpp>
#include <gui/popup/dialog_popup.hpp>
#include <logging/logging.hpp>
#include <gui/network_to_ui.hpp>
#include <translation/translation.hpp>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Ui::Ui(NetworkToUi* network_to_ui):
  font_path(Config::get("font_path", "C:/Work/pelaze/git/batajelo/data/fonts/").data()),
  theme_path(Config::get("theme_path", "C:/Work/pelaze/git/batajelo/data/themes/").data()),
  img_path(Config::get("img_path", "C:/Work/pelaze/git/batajelo/data/images/").data()),
  network_to_ui(network_to_ui)
{
  /**
   * @todo for default video mode :use the method getDesktopMode () instead of putting 800x600 
   **/
  if (sf::VideoMode(Config::get_int("width", 800), Config::get_int("height", 600)).isValid())
    this->window = new sf::RenderWindow(sf::VideoMode(Config::get_int("width", 800), Config::get_int("height", 600)), "Batajelo", this->get_display_mode());
  else
    this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Batajelo", this->get_display_mode());
  this->window->resetGLStates();
  this->window->setFramerateLimit(60);
  this->init_translation();
  this->set_event_handled(true);
  this->desktop = new sfg::Desktop();
  // cant add fontname in theme file it seems :x
  this->desktop->SetProperty("*", "FontName", this->font_path + "DroidSerif-BoldItalic.ttf");
  this->desktop->LoadThemeFromFile(theme_path + "login.theme");
  this->desktop->LoadThemeFromFile(theme_path + "settings.theme");
  this->desktop->LoadThemeFromFile(theme_path + "login_steps.theme");
  this->load_all_pages();
  this->load_all_popups();
  this->current_page->show();
}

Ui::~Ui()
{
  delete this->error_popup;
  delete this->login;
  delete this->settings;
  delete this->desktop;
  delete this->window;
}

void Ui::init_translation()
{
  this->gen.locale_cache_enabled(true);
  this->gen.add_messages_path("C:/Work/pelaze/git/batajelo/data/translation/");
  this->gen.add_messages_domain("strings");
  this->gen("FR.UTF-8");
  this->gen("EN.UTF-8");
  this->set_language(Config::get("language", "EN"));
}

void Ui::set_language(std::string language)
{
  /**
   * @TODO UTF8 doesnt work :/
   **/
  std::locale loc = this->gen(language + ".UTF-8");
  std::locale::global(loc);
  std::cout.imbue(loc);
}

void Ui::close()
{
  this->window->close();
}

bool Ui::run()
{
  return this->window->isOpen();
}

void Ui::clear()
{
  this->window->clear();
}

void Ui::refresh()
{
  if (this->clock.getElapsedTime().asMicroseconds() > 16666)
    {
      this->desktop->Update(this->clock.restart().asSeconds());
      this->clear();
      this->draw_background();
      this->draw_background_popup();
      this->display_ui();
      this->display_background();
      this->clock.restart();
    }
}

void Ui::translate()
{
  this->settings->translate();
  this->login->translate();
  this->login_steps->translate();
}

bool Ui::get_event_handled()
{
  return this->event_handled;
}

void Ui::set_event_handled(bool value)
{
  this->event_handled = value;
}

void Ui::handle_event()
{
  while(this->window->pollEvent(this->event))
  {
    this->event_handled = true;
    this->desktop->HandleEvent(this->event);
    if(this->event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) &&
      (this->current_page == this->login || this->current_page == this->settings ||
      this->current_page == this->login_steps)))
      this->close();
  }
}

void Ui::on_resize()
{
  this->desktop->UpdateViewRect(sf::FloatRect(0, 0, static_cast<float>(this->event.size.width), static_cast<float>(this->event.size.height)));
  this->settings->on_resize();
  this->login->on_resize();
  this->error_popup->on_resize();
}

void Ui::load_all_pages()
{
  this->login = new Login(this, this->desktop, this->window);
  this->login->activate();
  this->settings = new Settings(this, this->desktop, this->window);
  this->login_steps = new LoginSteps(this, this->desktop, this->window);
  this->current_page = this->login;
  this->current_popup = NULL;
}

void Ui::load_all_popups()
{
  this->error_popup = new ErrorPopup(this, this->desktop, this->window);
  this->dialog_popup = new DialogPopup(this, this->desktop, this->window);
}

void Ui::switch_to_page(Page* page)
{
  if (this->current_popup)
    {
      this->current_popup->hide();
      this->current_popup = NULL;
    }
  this->current_page->hide();
  page->activate();
  page->show();
  this->current_page = page;
}

void Ui::switch_to_error_popup(const std::string error)
{
  this->current_page->deactivate();
  this->current_popup = this->error_popup;
  this->error_popup->show(error);
}

void Ui::switch_to_dialog_popup(sfg::Box::Ptr box)
{
  this->current_page->deactivate();
  this->current_popup = this->dialog_popup;
  this->dialog_popup->show(box);
}

void Ui::switch_to_settings()
{
  this->switch_to_page(this->settings);
}

void Ui::switch_to_login()
{ 
  this->switch_to_page(this->login);
}

void Ui::switch_to_login_steps()
{
  this->switch_to_page(this->login_steps);
}

void Ui::draw_background()
{
  this->current_page->draw_background();
}

void Ui::draw_background_popup()
{
  if (this->current_popup != NULL)
    {
      this->current_popup->draw_background();
    }
}

void Ui::display_ui()
{
  this->sfgui.Display(*this->window);
}

void Ui::on_login_form_validated(const std::string& login,
                                 const std::string& password,
                                 const std::string& host,
                                 const short& port)
{
  this->network_to_ui->on_login_form_validated(login, password, host, port);
}

void Ui::display_background()
{
  this->window->display();
}

sf::Keyboard::Key Ui::get_key()
{
  return this->event.key.code;
}

void Ui::on_connection_failed()
{
  this->switch_to_error_popup(tr("Connection to server failed."));
}

void Ui::on_connection_success()
{
  this->login_steps->on_connection_success();
}

void Ui::on_authenticate(Command* received_command)
{
  log_debug("ON_AUTHENTICATE UI");
  static const std::string auth_messages[] = {"Success",
              tr("Unknown error."),
              tr("User does not exist.\n Please try again."),
              tr("Invalid password.\n Please try again."),
              tr("User already logged from an other location."),
              tr("Internal error.")};

  std::string result(received_command->body, received_command->body_size);
  int res = atoi(result.data());
  log_debug("on_authenticate :" << res << "." <<  ((res > 4) ? "Unknown error" : auth_messages[res]));
  if (res == 0)
    {
      this->login->on_authenticate_success();
      this->login_steps->on_authenticate_success();
    }
  else
    {
      this->switch_to_error_popup(auth_messages[res]);
    }
}

int Ui::get_display_mode()
{
  int display = Config::get_int("display", 2);

  if (display == 0)
    return sf::Style::Close;
  else if (display == 1)
    return sf::Style::Titlebar;
  return sf::Style::Fullscreen;
}
