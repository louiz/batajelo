#include <gui/ui.hpp>
#include <gui/settings.hpp>
#include <gui/home.hpp>
#include <gui/popup/popup.hpp>
#include <gui/popup/error_popup.hpp>
#include <logging/logging.hpp>
#include <game/game.hpp>

Ui::Ui(Game* game): game(game), img_path(Config::get("img_path", "C:/Work/pelaze/git/batajelo/data/images/").data()), 
	theme_path(Config::get("theme_path", "C:/Work/pelaze/git/batajelo/data/themes/").data()),
	font_path(Config::get("font_path", "C:/Work/pelaze/git/batajelo/data/fonts/").data())
{
	/**
	 *	@todo for default video mode :use the method getDesktopMode () instead of putting 800x600 
	 **/
	if (sf::VideoMode(Config::get_int("width", 800), Config::get_int("height", 600)).isValid())
		this->window = new sf::RenderWindow(sf::VideoMode(Config::get_int("width", 800), Config::get_int("height", 600)), "Batajelo", this->get_display_mode());
	else
		this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Batajelo", this->get_display_mode());
	this->window->resetGLStates();
	this->window->setFramerateLimit(60);
	this->desktop = new sfg::Desktop();
	// cant add fontname in theme file it seems :x
  this->desktop->SetProperty("*", "FontName", this->font_path + "DroidSerif-BoldItalic.ttf");
  this->desktop->LoadThemeFromFile(theme_path + "home.theme");
	this->load_all_pages();
	this->load_all_popups();
	this->current_page->show();
}

Ui::~Ui() 
{
	delete this->error_popup;
	delete this->home;
	delete this->settings;
	delete this->desktop;
	delete this->window;
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

void Ui::handleEvent()
{
	while(this->window->pollEvent(this->event)) 
	{
		this->desktop->HandleEvent(this->event);
		if(this->event.type == sf::Event::Closed) 
		{
			this->close();
		}
		else if(this->event.type == sf::Event::Resized) 
		{
		}
	}
}

void Ui::load_all_pages()
{
	this->home = new Home(this, this->desktop, this->window);
	this->home->activate();
	this->settings = new Settings(this, this->desktop, this->window);
	this->current_page = this->home;
	this->current_popup = NULL;
}

void Ui::load_all_popups()
{
	this->error_popup = new ErrorPopup(this, this->desktop, this->window);
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

void Ui::switch_to_settings()
{ 
	this->switch_to_page(this->settings);
}

void Ui::switch_to_home()
{ 
	this->switch_to_page(this->home);
}

/**
 * @todo maybe we can optimize by calculating the resize at starting and only when window size are changed
 **/
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
	this->game->on_login_form_validated(login, password, host, port);
}

void Ui::display_background()
{
	this->window->display();
}

sf::Keyboard::Key Ui::get_key()
{
	return this->event.key.code;
}

void Ui::on_authenticate(Command* received_command)
{
	log_debug("ON_AUTHENTICATE UI");
	static const std::string auth_messages[] = {"Success",
				      "Unknown error",
				      "User does not exist",
				      "Invalid password",
              "User already logged from an other location"};

  std::string result(received_command->body, received_command->body_size);
  int res = atoi(result.data());
  log_debug("on_authenticate :" << res << "." <<  ((res > 4) ? "Unknown error" : auth_messages[res]));
  if (res == 0)
    {
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
		return sf::Style::None;
	return sf::Style::Fullscreen;
}