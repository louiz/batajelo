#include <ui/settings.hpp>
#include <ui/ui.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>
#include <boost/algorithm/string.hpp>

Settings::Settings(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): Page(ui, desktop, window) 
{
	this->build_ui();
}

void Settings::hide()
{
	this->img_back->Show(false);
	this->window_settings->Show(false);
}

void Settings::show()
{
	this->img_back->Show(true);
	this->window_settings->Show(true);
}

void Settings::update() 
{
	std::vector<std::string> choosen_res;
	boost::split(choosen_res, this->res->GetSelectedText().toAnsiString(), boost::is_any_of("x"));
	this->window->setSize(sf::Vector2u(atoi(choosen_res.at(0).c_str()), atoi(choosen_res.at(1).c_str())));
}

void Settings::draw_background()
{
  sf::Texture bg;
  if (!bg.loadFromFile(this->ui->img_path + Config::get("home_bg", "bg.jpg").data()))
  {
    log_error("cant load home background");
  }
  sf::Sprite sprite_bg(bg);

  this->window->draw(sprite_bg);
}

void Settings::build_ui()
{
	// Back button 
	this->img_back = sfg::Image::Create();
	sf::Image img;
	if(img.loadFromFile(this->ui->img_path + "back.png")) 
	{
		this->img_back->SetImage(img);
		this->img_back->SetAllocation(sf::FloatRect(10, 10, 10, 10));
		this->img_back->OnLeftClick.Connect(&Ui::switch_to_home, this->ui);
		this->img_back->Show(false);
	} 
	else
	{
		log_error("cant load img : settings");
	}

	// Resolution comboBox 
  sfg::Label::Ptr label_res = sfg::Label::Create("Resolution : ");
  label_res->SetId("label_res");
	this->res = sfg::ComboBox::Create();
	this->res->AppendItem("800x600");
	this->res->AppendItem("1024x768");
	this->res->AppendItem("1280x1024");
	this->res->AppendItem("1600x1200");

	// Graphics comboBox 
	sfg::Label::Ptr label_graphics = sfg::Label::Create("Graphics : ");
  label_graphics->SetId("label_graphics");
	this->graphics = sfg::ComboBox::Create();
	this->graphics->AppendItem("low");
	this->graphics->AppendItem("medium");
	this->graphics->AppendItem("ultra");

	// Button event 
  sfg::Button::Ptr button_settings(sfg::Button::Create("Ok"));
  button_settings->SetId("button_settings");
  button_settings->OnLeftClick.Connect(&Settings::update, this);

	// Create box 
  sfg::Box::Ptr box_settings(sfg::Box::Create(sfg::Box::VERTICAL));
  box_settings->SetId("box_settings");
	box_settings->Pack(label_res);
  box_settings->Pack(res);
	box_settings->Pack(label_graphics);
  box_settings->Pack(graphics);
  box_settings->SetSpacing(10);
	box_settings->Pack(button_settings);

	// Settings window 
	this->window_settings = sfg::Window::Create(sfg::Window::BACKGROUND);
  this->window_settings->SetId("window_settings");
  this->window_settings->Add(box_settings);
	this->window_settings->Show(false);
	this->window_settings->SetAllocation(sf::FloatRect(320, 80, 200, 200));

	this->desktop->Add(this->img_back);
	this->desktop->Add(this->window_settings);
}