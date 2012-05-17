#include <ui/home.hpp>
#include <ui/ui.hpp>
#include <logging/logging.hpp>
#include <config/config.hpp>

Home::Home(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): Page(ui, desktop, window) 
{
	this->build_ui();
}

void Home::connect() 
{
	if (this->entry_login->GetText() != "" && this->entry_pass->GetText() != "")
	{
	//	std::cout <<  md5.digestString(const_cast<char *>(this->entry_pass->GetText().toAnsiString().c_str())) << std::endl;
		std::cout << this->entry_login->GetText().toAnsiString() << std::endl;
		this->ui->on_login_form_validated(this->entry_login->GetText().toAnsiString(), this->entry_pass->GetText().toAnsiString(), "127.0.0.1", 7878);
	}
}

void Home::hide()
{
	this->img_settings->Show(false);
	this->window_login->Show(false);
}

void Home::show()
{
	this->img_settings->Show(true);
	this->window_login->Show(true);
}

void Home::draw_background()
{
	sf::Texture bg;

  if (!bg.loadFromFile(this->ui->img_path + Config::get("home_bg", "bg.jpg").data()))
  {
    log_error("cant load home background");
  }
	else	
	{
		sf::Sprite sprite_bg(bg);
		this->window->draw(sprite_bg);
	}
}

void Home::build_ui()
{
  // Settings button 
	this->img_settings = sfg::Image::Create();
	sf::Image img;
	if(img.loadFromFile(this->ui->img_path + "settings.png")) 
	{
		this->img_settings->SetImage(img);
		this->img_settings->SetAllocation(sf::FloatRect(10, 10, 10, 10));		
		this->img_settings->OnLeftClick.Connect(&Ui::switch_to_settings, this->ui);
	} 
	else	
	{
		log_error("cant load img : settings");
	}

  // Button event 
  sfg::Button::Ptr button_connect(sfg::Button::Create("Connexion"));
  button_connect->SetId("button_connect");
  button_connect->OnLeftClick.Connect(&Home::connect, this);

  // Labels 
  label_login = sfg::Label::Create("Login");
  label_login->SetId("label_login");
  label_pass = sfg::Label::Create("Password");
  label_pass->SetId("label_pass");

  //Entries 
  this->entry_login = sfg::Entry::Create();
  this->entry_login->SetId("entry_login");
  this->entry_login->SetRequisition(sf::Vector2f(80, 0));
  this->entry_pass = sfg::Entry::Create();
  this->entry_pass->SetId("entry_password");
  this->entry_pass->SetRequisition(sf::Vector2f(80, 0));
  this->entry_pass->HideText('*');

  // Create box 
  sfg::Box::Ptr box_connect(sfg::Box::Create(sfg::Box::VERTICAL));
  box_connect->SetId("box_connect");
  box_connect->Pack(label_login);
  box_connect->Pack(entry_login);
  box_connect->Pack(label_pass);
  box_connect->Pack(entry_pass);
  box_connect->SetSpacing(10);
  box_connect->Pack(button_connect, false);

  // Create login 
	this->window_login = sfg::Window::Create(sfg::Window::BACKGROUND);
  this->window_login->SetId("window_login");
  this->window_login->Add(box_connect);
  this->window_login->SetPosition(sf::Vector2f(320, 80));

  // Add to desktop
	this->desktop->Add(this->img_settings);
  this->desktop->Add(this->window_login);
}

void Home::on_authenticate_error(const char* error)
{
	log_error("home : " << error);
}