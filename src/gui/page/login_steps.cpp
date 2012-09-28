#include <gui/page/login_steps.hpp>
#include <gui/ui.hpp>
#include <logging/logging.hpp>
#include <config/config.hpp>
#include <SFML/System.hpp>
#include <translation/translation.hpp>

LoginSteps::LoginSteps(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): Page(ui, desktop, window)
{
  this->build_ui();
  this->build_background();
}

void LoginSteps::hide()
{
  this->window_steps->Show(false);
}

void LoginSteps::show()
{
  this->init();
  this->window_steps->Show(true);
}

void LoginSteps::deactivate()
{
  this->window_steps->SetState(sfg::Widget::INSENSITIVE);
}

void LoginSteps::activate()
{
  this->window_steps->SetState(sfg::Widget::NORMAL);
}

void LoginSteps::on_resize()
{
   this->window_steps->SetPosition(sf::Vector2f(800.f*Config::get_float("width", 800.f)/1920.f, 400.f*Config::get_float("height", 600.f)/1080.f));
  // this->window_steps->SetAl(sf::FloatRect(700*Config::get_int("width", 800)/1920, 80*Config::get_int("height", 600)/600, 100*Config::get_int("width", 800)/800, 100*Config::get_int("height", 600)/600));
}

void LoginSteps::translate()
{
  this->first_step->SetText(tr("Connecting to server..."));
  this->second_step->SetText(tr("Authentification..."));
  this->cancel_connect->SetLabel(tr("Cancel"));
}

void LoginSteps::build_background()
{
  /*
  if (!this->bg.loadFromFile(this->ui->img_path + Config::get("home_bg", "bg.jpg").data()))
  {
    log_error("cant load LoginSteps background");
  }ufioruferioffu
  else
  {
    this->sprite_bg.setTexture(this->bg);
    this->sprite_bg.setScale((float)Config::get_int("width", 800)/1920, (float)Config::get_int("height", 600)/1080);
  }*/
  this->bg.setSize(sf::Vector2f(Config::get_float("width", 800.f), Config::get_float("height", 600.f)));
  this->bg.setFillColor(sf::Color(128,128,128,100));
}

void LoginSteps::draw_background()
{
  this->window->draw(this->sprite_bg);
}

void LoginSteps::build_ui()
{
  // Login steps
  this->first_step = sfg::Label::Create(tr("Connecting to server..."));
  this->second_step = sfg::Label::Create(tr("Authentification..."));
  // Cancel button
  this->cancel_connect = sfg::Button::Create(tr("Cancel"));
  this->cancel_connect->GetSignal(sfg::Widget::OnLeftClick).Connect(&Ui::switch_to_login, this->ui);
  // Table
  sfg::Table::Ptr table_login = sfg::Table::Create();
  table_login->SetRowSpacings(50);
  table_login->Attach(this->first_step, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_login->Attach(this->second_step, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_login->Attach(this->cancel_connect, sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  // Window
  this->window_steps = sfg::Window::Create(sfg::Window::BACKGROUND);
  this->window_steps->SetId("window_login_steps");
  this->window_steps->Add(table_login);
  this->window_steps->SetPosition(sf::Vector2f(800.f*Config::get_float("width", 800.f)/1920.f, 400.f*Config::get_float("height", 600.f)/1080.f));
  this->window_steps->Show(false);
  // Desktop
  this->desktop->Add(this->window_steps);
 }

void LoginSteps::init()
{
  this->first_step->SetText(tr("Connecting to server..."));
  this->second_step->SetText(tr("Authentification..."));
}

void LoginSteps::on_authenticate_success()
{
  this->ui->get_teams_stats();
  this->second_step->SetText(tr("Authentification succeed"));
  this->ui->switch_to_home();
}

void LoginSteps::on_connection_success()
{
  this->first_step->SetText(tr("Connected."));
}