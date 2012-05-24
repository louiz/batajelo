#include <gui/home.hpp>
#include <gui/ui.hpp>
#include <logging/logging.hpp>
#include <config/config.hpp>
#include <SFML/System.hpp>
#include <boost/bind.hpp>

Home::Home(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): Page(ui, desktop, window)
{
  this->build_ui();
  this->build_background();
}
/*

void Home::loading_animate()
{
  char num[21];
  this->img_loading->Show(true);
  int i;
  // THAT SHIT IS TOO FAST
  for (int next_animation = 0; next_animation <= 7; next_animation++)
  {
    i = 0;
    for (; i != 100; i++);
    itoa(next_animation, num, 10);
    if(this->img.loadFromFile(this->ui->img_path + "gifs/loading/loading_"+num+".png"))
    {
      this->img_loading->SetImage(img);
    }
    else
    {
      log_error("cant load img : " << "loading_" << itoa(i, num, 10) << ".png");
    }
  }
}*/

void Home::connect()
{
  if (this->entry_login->GetText() != "" && this->entry_pass->GetText() != "")
  {
    //this->loading_animate();
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

void Home::deactivate()
{
  this->window_login->SetState(sfg::Widget::INSENSITIVE);
  this->img_settings->SetState(sfg::Widget::INSENSITIVE);
}

void Home::activate()
{
  this->window_login->SetState(sfg::Widget::NORMAL);
  this->img_settings->SetState(sfg::Widget::NORMAL);
}

void Home::build_background()
{
  if (!this->bg.loadFromFile(this->ui->img_path + Config::get("home_bg", "bg.jpg").data()))
  {
    log_error("cant load home background");
  }
  else
  {
    this->sprite_bg.setTexture(this->bg);
    this->sprite_bg.setScale((float)Config::get_int("width", 800)/1920, (float)Config::get_int("height", 600)/1080);
  }
}

void Home::draw_background()
{
  this->window->draw(this->sprite_bg);
}

void Home::build_ui()
{
  // Settings button
  this->img_settings = sfg::Image::Create();
  if(this->img.loadFromFile(this->ui->img_path + "settings.png"))
  {
    this->img_settings->SetImage(img);
    this->img_settings->SetAllocation(sf::FloatRect(10, 10, 10, 10));
    this->img_settings->GetSignal(sfg::Widget::OnLeftClick).Connect(&Ui::switch_to_settings, this->ui);
    this->img_settings->Show(false);
  }
  else
  {
    log_error("cant load img : settings");
  }

  // Button event
  this->button_connect = sfg::Button::Create("Connexion");
  this->button_connect->SetId("button_connect");
  this->button_connect->GetSignal(sfg::Widget::OnLeftClick).Connect(&Home::connect, this);
  this->button_connect->GetSignal(sfg::Widget::OnKeyPress).Connect(&Home::handle_keys_connect, this);

  // Labels
  sfg::Label::Ptr label_login = sfg::Label::Create("Login");
  label_login->SetId("label_login");
  sfg::Label::Ptr label_pass = sfg::Label::Create("Password");
  label_pass->SetId("label_pass");

  //Entries
  this->entry_login = sfg::Entry::Create();
  entry_login->SetId("entry_login");
  entry_login->SetRequisition(sf::Vector2f(80, 0));
  entry_login->GetSignal(sfg::Widget::OnExpose).Connect(&Home::handle_keys_login, this);

  entry_pass = sfg::Entry::Create();
  entry_pass->SetId("entry_password");
  entry_pass->SetRequisition(sf::Vector2f(80, 0));
  entry_pass->HideText('*');
  entry_pass->GetSignal(sfg::Widget::OnKeyPress).Connect(&Home::handle_keys_password, this);

  // Settings button
  this->img_loading = sfg::Image::Create();
  this->img_loading->Show(false);

  // Create box
  sfg::Box::Ptr box_connect(sfg::Box::Create(sfg::Box::VERTICAL));
  box_connect->SetId("box_connect");
  box_connect->Pack(label_login);
  box_connect->Pack(this->entry_login);
  box_connect->Pack(label_pass);
  box_connect->Pack(this->entry_pass);
  box_connect->SetSpacing(10);
  box_connect->Pack(this->button_connect, false);
  // box_connect->Pack(this->img_loading);

  // Create login
  this->window_login = sfg::Window::Create(sfg::Window::BACKGROUND);
  this->window_login->SetId("window_login");
  this->window_login->Add(box_connect);
  this->window_login->SetPosition(sf::Vector2f(320, 80));
  this->window_login->Show(false);

  // Add to desktop
  this->desktop->Add(this->img_settings);
  this->desktop->Add(this->window_login);
}

void Home::handle_keys_login()
{
  if (this->ui->get_key() == sf::Keyboard::Tab && this->entry_login->HasFocus())
  {
    this->entry_pass->GrabFocus();
  }
}

void Home::handle_keys_password()
{
  if (this->ui->get_key() == sf::Keyboard::Tab)
  {
    this->button_connect->GrabFocus();
  }
}

void Home::handle_keys_connect()
{
  if (this->ui->get_key() == sf::Keyboard::Return)
  {
    boost::bind(&Home::connect, this);
  }
}
