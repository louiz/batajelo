#include <ui/ui.hpp>
#include <ui/settings.hpp>
#include <ui/home.hpp>
#include <logging/logging.hpp>
#include <game/game.hpp>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Ui::Ui(Game* game): game(game)
{
	this->window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Batajelo", sf::Style::Close);
	this->window->resetGLStates();
	this->desktop = new sfg::Desktop();
	this->font_path = Config::get("font_path", "C:/Work/pelaze/git/experiment/new client/data/fonts/").data();
	this->theme_path = Config::get("theme_path", "C:/Work/pelaze/git/experiment/new client/data/themes/").data();
  this->img_path = Config::get("img_path", "C:/Work/pelaze/git/experiment/new client/data/images/").data();
	// cant add fontname in theme file it seems :x
  this->desktop->SetProperty("*", "FontName", this->font_path + "DroidSerif-BoldItalic.ttf");
  this->desktop->LoadThemeFromFile(theme_path + "home.theme");
	this->load_all_pages();
}

Ui::~Ui() 
{
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

void Ui::update()
{		
	this->desktop->Update(this->clock.restart().asSeconds());
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
	this->settings = new Settings(this, this->desktop, this->window);
	this->current_page = this->home;
}

void Ui::switch_to_page(Page* page)
{
	this->current_page->hide();
	page->show();
	this->current_page = page;
}

void Ui::switch_to_settings()
{ 
	this->switch_to_page(this->settings);
}

void Ui::switch_to_home()
{ 
	this->switch_to_page(this->home);
}

void Ui::draw_background()
{
	this->current_page->draw_background();
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

void Ui::on_authenticate(Command* received_command)
{
	log_debug("ON_AUTHENTICATE UI");
	static const char* auth_messages[] = {"Success",
				      "Unknown error",
				      "User does not exist",
				      "Invalid password",
              "User already logged from an other location"};

  std::string result(received_command->body, received_command->body_size);
  int res = atoi(result.data());
  log_debug("on_authenticate :" << res << "." <<  ((res > 4) ? "Unknown error" : auth_messages[res]));
  if (res == 0)
    {
   //   this->request_file("file.bin");
   //   this->ui.login_success();
    }
	else
		{
			this->home->on_authenticate_error(auth_messages[res]);
	//		this->ui.login_error(auth_messages[res]);
		}
}
