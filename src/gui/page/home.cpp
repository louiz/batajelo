#include <gui/page/home.hpp>
#include <gui/ui.hpp>
#include <logging/logging.hpp>
#include <config/config.hpp>
#include <translation/translation.hpp>

Home::Home(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): Page(ui, desktop, window)
{
  this->build_ui();
  this->build_background();
}

void Home::hide()
{
  this->window_home->Show(false);
}

void Home::show()
{
  this->window_home->Show(true);
}

void Home::deactivate()
{
  this->window_home->SetState(sfg::Widget::INSENSITIVE);
}

void Home::activate()
{
  this->window_home->SetState(sfg::Widget::NORMAL);
}

void Home::on_resize()
{
 // this->window_home->SetPosition(sf::Vector2f(750.f*Config::get_int("width", 800)/1920.f, 400.f*Config::get_int("height", 600)/1080.f));
 // this->sprite_bg.setScale(Config::get_int("width", 800)/1920.f, Config::get_int("height", 600)/1080.f);
}

void Home::translate()
{
}

void Home::draw_background()
{
//  this->window->draw(this->sprite_bg);
//  this->window->draw(this->bg);
    this->window->draw(this->bg);
}

void Home::build_background()
{
  this->bg.setSize(sf::Vector2f(Config::get_float("width", 800.f), Config::get_float("height", 600.f)));
  this->bg.setFillColor(sf::Color(128,128,128,0));
}

void Home::build_ui()
{
  /**
   * @TODO use a method to get the stats/league of a user
   **/
  sf::Image img;
  this->img_league = sfg::Image::Create();
  if (img.loadFromFile(this->ui->img_path + "bronze.png"))
    {
      this->img_league->SetImage(img);
   //   this->img_league->GetSignal(sfg::Widget::OnLeftClick).Connect(&Ladder::display, this);
      this->img_league->Show();
    }
  else
    log_error("cant load img league");
  sfg::Label::Ptr label_league = sfg::Label::Create();
  label_league->SetText("Bronze League");
  sfg::Label::Ptr label_login = sfg::Label::Create();
  label_login->SetId("label_login");
  label_login->SetText(this->ui->current_user->get_key("login"));
  // Header Table
  sfg::Table::Ptr table_header = sfg::Table::Create();
  table_header->SetId("table_header");
  table_header->SetRequisition(sf::Vector2f(Config::get_float("width", 800.f), 150.f*Config::get_float("height", 600.f)/1080.f));
  table_header->Attach(this->img_league, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_header->SetColumnSpacing(0, 50.f*Config::get_float("height", 600.f)/1080.f);
  table_header->Attach(label_login, sf::Rect<sf::Uint32>(1, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_header->SetRowSpacing(0, 20.f*Config::get_float("height", 600.f)/1080.f);
  table_header->Attach(label_league, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  // Separator Header <-> Body
  sfg::Separator::Ptr separator_header = sfg::Separator::Create();
  // Ladder table
  sfg::Table::Ptr table_ladder = sfg::Table::Create();
  table_ladder->SetId("table_ladder");
  table_ladder->SetRowSpacings(50.f*Config::get_float("height", 600.f)/1080.f);
  table_ladder->SetColumnSpacings(75.f*Config::get_float("height", 600.f)/1080.f);
  sfg::Button::Ptr button_first_mode = sfg::Button::Create(tr("1v1"));
  sfg::Button::Ptr button_second_mode = sfg::Button::Create(tr("2v2"));
  sfg::Button::Ptr button_third_mode = sfg::Button::Create(tr("3v3"));
  sfg::Button::Ptr button_fourth_mode = sfg::Button::Create(tr("4v4"));
  sfg::Button::Ptr button_first_race = sfg::Button::Create(tr("Race jaune"));
  sfg::Button::Ptr button_second_race = sfg::Button::Create(tr("Race noire"));
  sfg::Button::Ptr button_third_race = sfg::Button::Create(tr("Race blanche"));
  table_ladder->Attach(button_first_mode, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_ladder->Attach(button_second_mode, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_ladder->Attach(button_third_mode, sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_ladder->Attach(button_fourth_mode, sf::Rect<sf::Uint32>(0, 3, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_ladder->Attach(button_first_race, sf::Rect<sf::Uint32>(1, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_ladder->Attach(button_second_race, sf::Rect<sf::Uint32>(2, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_ladder->Attach(button_third_race, sf::Rect<sf::Uint32>(3, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  // Body table
  sfg::Table::Ptr table_body = sfg::Table::Create();
  sfg::Table::Ptr table_stats = sfg::Table::Create();
  sfg::Notebook::Ptr notebook_body = sfg::Notebook::Create();
  notebook_body->SetId("notebook_body");
  notebook_body->SetRequisition(sf::Vector2f(Config::get_float("width", 800.f), 650.f*Config::get_float("height", 600.f)/1080.f));
  sfg::Label::Ptr label_ladder = sfg::Label::Create(tr("Ladder"));
  sfg::Label::Ptr label_stats = sfg::Label::Create(tr("Stats"));
  notebook_body->AppendPage(table_ladder, label_ladder);
  notebook_body->AppendPage(table_stats, label_stats);
  table_body->Attach(notebook_body, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  // Footer table
  sfg::Table::Ptr table_footer = sfg::Table::Create();
  table_footer->SetRequisition(sf::Vector2f(Config::get_float("width", 800.f), 200.f*Config::get_float("height", 600.f)/1080.f));
  sfg::Button::Ptr button_europe_chan = sfg::Button::Create(tr("Channel Europe"));
  sfg::Button::Ptr button_pm = sfg::Button::Create(tr("Privates messages"));
  sfg::Button::Ptr button_friend_one = sfg::Button::Create("friend 1");
	sfg::ScrolledWindow::Ptr chat_window = sfg::ScrolledWindow::Create();
  chat_window->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_NEVER|sfg::ScrolledWindow::VERTICAL_ALWAYS);
  sfg::Entry::Ptr chat_entry = sfg::Entry::Create();
//  chat_entry->SetRequisition(sf::Vector2f(1300.f, 0.f));
  table_footer->Attach(button_europe_chan, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_footer->Attach(button_pm, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_footer->Attach(chat_window, sf::Rect<sf::Uint32>(1, 0, 1, 4), sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
  table_footer->Attach(chat_entry, sf::Rect<sf::Uint32>(1, 4, 1, 1), sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
  table_footer->Attach(button_friend_one, sf::Rect<sf::Uint32>(2, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  // Main table
  sfg::Table::Ptr table_home = sfg::Table::Create();
  table_home->SetId("table_home");
  table_home->Attach(table_header, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_home->Attach(table_body, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table_home->Attach(table_footer, sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  // Window
  this->window_home = sfg::Window::Create(sfg::Window::BACKGROUND);
  this->window_home->SetId("window_home");
  this->window_home->Add(table_home);
  this->window_home->Show(false);
  // Desktop
  this->desktop->Add(this->window_home);
}

void Home::on_get_stats()
{

}