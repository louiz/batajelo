#include <gui/page/login.hpp>
#include <gui/ui.hpp>
#include <logging/logging.hpp>
#include <config/config.hpp>
#include <SFML/System.hpp>
#include <functional>
#include <sha.h>
#include <base64.h>
#include <translation/translation.hpp>

Login::Login(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): Page(ui, desktop, window)
{
  this->build_ui();
  this->build_background();
}

void Login::connect()
{
  std::string login = this->entry_login->GetText().toAnsiString();
  std::string password = this->entry_pass->GetText().toAnsiString();
  std::string digest;
  CryptoPP::SHA256 hash;

  if (this->entry_login->GetText() != "" && password != "")
    {
      this->ui->switch_to_login_steps();
      CryptoPP::StringSource(password, true, new CryptoPP::HashFilter(hash, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(digest))));
      this->ui->on_login_form_validated(this->entry_login->GetText().toAnsiString(), digest, "127.0.0.1", 7878);
    }
  else
    {
      this->ui->switch_to_error_popup(tr("Insert your account name and password."));
    }
}

void Login::hide()
{
  this->window_menu->Show(false);
  this->window_login->Show(false);
  this->remember_login->Show(false);
}

void Login::show()
{
  this->window_menu->Show(true);
  this->window_login->Show(true);
  this->remember_login->Show(true);
}

void Login::deactivate()
{
  this->window_menu->SetState(sfg::Widget::INSENSITIVE);
  this->window_login->SetState(sfg::Widget::INSENSITIVE);
  this->remember_login->SetState(sfg::Widget::INSENSITIVE);
}

void Login::activate()
{
  this->window_menu->SetState(sfg::Widget::NORMAL);
  this->window_login->SetState(sfg::Widget::NORMAL);
  this->remember_login->SetState(sfg::Widget::NORMAL);
}

void Login::on_resize()
{
  this->window_menu->SetPosition(sf::Vector2f(100.f*Config::get_float("width", 800.f)/1920.f, 800.f*Config::get_float("height", 600.f)/1080.f));
  this->window_login->SetPosition(sf::Vector2f(750.f*Config::get_float("width", 800.f)/1920.f, 400.f*Config::get_float("height", 600.f)/1080.f));
  this->sprite_bg.setScale(Config::get_float("width", 800.f)/1920.f, Config::get_float("height", 600.f)/1080.f);
}

void Login::translate()
{
  this->label_login->SetText(tr("Account Name"));
  this->label_pass->SetText(tr("Password"));
  this->button_login->SetLabel(tr("Login"));
  this->remember_login->SetLabel(tr("Remember my account name"));
}

void Login::invalidate()
{
  this->desktop->LoadThemeFromFile("C:/Work/pelaze/git/batajelo/data/themes/Login.theme");
  this->entry_login->Invalidate();
  this->entry_pass->Invalidate();
  this->window_login->Invalidate();
}

void Login::build_background()
{/*
  if (!this->bg.loadFromFile(this->ui->img_path + Config::get("Login_bg", "bg.jpg").data()))
  {
    log_error("cant load Login background");
  }
  else
  {
    this->sprite_bg.setTexture(this->bg, true);
    this->sprite_bg.setScale((float)Config::get_int("width", 800)/1920, (float)Config::get_int("height", 600)/1080);
    */
  this->bg.setSize(sf::Vector2f(Config::get_float("width", 800.f), Config::get_float("height", 600.f)));
  this->bg.setFillColor(sf::Color(128,128,128,100));
}

void Login::draw_background()
{
//  this->window->draw(this->sprite_bg);
  this->window->draw(this->bg);
}

void Login::build_ui()
{
  // Refresh button
  sf::Image img;
  this->img_refresh = sfg::Image::Create();
  if(img.loadFromFile(this->ui->img_path + "refresh.png"))
    {
      this->img_refresh->SetImage(img);
      this->img_refresh->SetPosition(sf::Vector2f(300.f, 0.f));
      this->img_refresh->GetSignal(sfg::Widget::OnLeftClick).Connect(&Login::invalidate, this);
      this->img_refresh->Show();
    }
  else
    log_error("cant load img : refresh");
  // Labels
  this->label_login = sfg::Label::Create(tr("Account name"));
  this->label_login->SetId("label_login");
  this->label_pass = sfg::Label::Create(tr("Password"));
  this->label_pass->SetId("label_pass");
  //Entries
  this->entry_login = sfg::Entry::Create();
  this->entry_login->SetId("entry_login");
  this->entry_login->SetText(Config::get("login", ""));
  this->entry_login->SetRequisition(sf::Vector2f(80.f, 0.f));
  this->entry_login->GetSignal(sfg::Widget::OnKeyPress).Connect(&Login::handle_keys_login, this);
  this->entry_pass = sfg::Entry::Create();
  this->entry_pass->SetId("entry_password");
  this->entry_pass->HideText('*');
  this->entry_pass->GetSignal(sfg::Widget::OnKeyPress).Connect(&Login::handle_keys_password, this);
  // Buttons
  this->button_login = sfg::Button::Create(tr("Login"));
  this->button_login->SetId("button_login");
  this->button_login->GetSignal(sfg::Widget::OnLeftClick).Connect(&Login::connect, this);
  this->button_login->GetSignal(sfg::Widget::OnKeyPress).Connect(&Login::handle_keys_connect, this);
  this->button_settings = sfg::Button::Create(tr("Settings"));
  this->button_settings->GetSignal(sfg::Widget::OnLeftClick).Connect(&Ui::switch_to_settings, this->ui);
  this->button_close = sfg::Button::Create(tr("Quit"));
  this->button_close->GetSignal(sfg::Widget::OnLeftClick).Connect(&Ui::close, this->ui);
  // Save login
  this->remember_login = sfg::CheckButton::Create(tr("Remember my account name"));
  this->remember_login->SetId("button_remember_login");
  // Create tables
  sfg::Table::Ptr table_login = sfg::Table::Create();
  table_login->SetId("table_login");
  table_login->SetRowSpacings(20);
  table_login->Attach(this->label_login, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_login->Attach(this->entry_login, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_login->Attach(this->label_pass, sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_login->Attach(this->entry_pass, sf::Rect<sf::Uint32>(0, 3, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_login->Attach(this->button_login, sf::Rect<sf::Uint32>(0, 4, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_login->SetRowSpacing(4, 50);
  table_login->Attach(this->remember_login, sf::Rect<sf::Uint32>(0, 5, 1, 1), sfg::Table::NORMAL, sfg::Table::NORMAL);
  sfg::Table::Ptr table_menu = sfg::Table::Create();
  table_menu->SetId("table_menu");
  table_menu->SetRowSpacings(20);
  table_menu->Attach(this->button_settings, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_menu->Attach(this->button_close, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  // Create windows
  this->window_login = sfg::Window::Create(sfg::Window::NO_STYLE);
  this->window_login->SetId("window_login");
  this->window_login->Add(table_login);
  this->window_login->SetPosition(sf::Vector2f(750.f*Config::get_float("width", 800.f)/1920.f, 400.f*Config::get_float("height", 600.f)/1080.f));
  this->window_login->Show(false);
  this->window_menu = sfg::Window::Create(sfg::Window::NO_STYLE);
  this->window_menu->SetId("window_menu");
  this->window_menu->Add(table_menu);
  this->window_menu->SetPosition(sf::Vector2f(100.f*Config::get_float("width", 800.f)/1920.f, 800.f*Config::get_float("height", 600.f)/1080.f));
  this->window_menu->Show(false);
  // Desktop
  this->desktop->Add(this->img_refresh);
  this->desktop->Add(this->window_menu);
  this->desktop->Add(this->window_login);
}

void Login::handle_keys_login()
{
  if (this->ui->get_key() == sf::Keyboard::Tab && this->ui->get_event_handled() == true)
    {
      this->ui->set_event_handled(false);
      this->entry_pass->GrabFocus();
    }
}

void Login::handle_keys_password()
{
  if (this->ui->get_key() == sf::Keyboard::Tab && this->ui->get_event_handled() == true)
    {
      this->ui->set_event_handled(false);
      this->button_login->GrabFocus();
      this->button_login->SetState(sfg::Widget::PRELIGHT);
    }
}

void Login::handle_keys_connect()
{
  if (this->ui->get_key() == sf::Keyboard::Return)
    {
      this->ui->set_event_handled(false);
      this->connect();
    }
  else if (this->ui->get_key() == sf::Keyboard::Tab && this->ui->get_event_handled() == true)
    {
      this->ui->set_event_handled(false);
      this->entry_login->GrabFocus();
    }
}

void Login::on_authenticate_success()
{
  if (this->remember_login->IsActive())
    {
      Config::set("login", this->entry_login->GetText().toAnsiString(), true);
    }
}
