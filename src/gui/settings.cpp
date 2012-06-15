#include <gui/settings.hpp>
#include <gui/ui.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>
#include <boost/algorithm/string.hpp>
#include <translation/translation.hpp>

Settings::Settings(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): Page(ui, desktop, window)
{
  this->build_languages();
  this->build_ui();
  this->build_background();
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

void Settings::deactivate()
{
  this->img_back->SetState(sfg::Widget::INSENSITIVE);
  this->window_settings->SetState(sfg::Widget::INSENSITIVE);
}

void Settings::change_resolution()
{
  if (this->display->GetSelectedItem() == 1)
    {
      this->res->SetState(sfg::Widget::INSENSITIVE);
      this->res->SelectItem(0);
    }
  else
    this->res->SetState(sfg::Widget::NORMAL);
}

void Settings::activate()
{
  this->img_back->SetState(sfg::Widget::NORMAL);
  this->window_settings->SetState(sfg::Widget::NORMAL);
}

void Settings::on_resize()
{
  this->window_settings->SetAllocation(sf::FloatRect(300.f*Config::get_int("width", 600)/1920.f, 200.f*Config::get_int("height", 600)/1080.f, 1200.f*Config::get_int("width", 600)/1920.f, 600.f*Config::get_int("height", 600)/1080.f));
 // this->window_settings->SetPosition(sf::Vector2f(300.f*Config::get_int("width", 800)/1920.f, 200.f*Config::get_int("height", 600)/1080.f));
  this->sprite_bg.setScale(Config::get_int("width", 800)/1920.f, Config::get_int("height", 600)/1080.f);
}

void Settings::build_background()
{
/*  if (!this->bg.loadFromFile(this->ui->img_path + Config::get("home_bg", "bg.jpg").data()))
    {
      log_error("cant load home background");
    }
  else
    {
      this->sprite_bg.setTexture(this->bg);
      this->sprite_bg.setScale((float)Config::get_int("width", 800)/1920, (float)Config::get_int("height", 600)/1080);
    }*/
  this->bg.setSize(sf::Vector2f(Config::get_int("width", 800), Config::get_int("height", 600)));
  this->bg.setFillColor(sf::Color(128,128,128,100));
}

void Settings::draw_background()
{
  this->window->draw(this->bg);
}

void Settings::translate()
{
  this->build_languages();
  this->label_table_video->SetText(tr("Video"));
  this->label_table_account->SetText(tr("Account"));
  this->label_display->SetText(tr("Display"));
  this->display->ChangeItem(0, tr("Windowed"));
  this->display->ChangeItem(1, tr("Windowed (fullscreen)"));
  this->display->ChangeItem(2, tr("Fullscreen"));
  this->label_res->SetText(tr("Resolution"));
  this->label_language->SetText(tr("Language"));
  this->language->ChangeItem(0, tr("English"));
  this->language->ChangeItem(1, tr("French"));
  this->language->ChangeItem(2, tr("German"));
  /**
   * @TODO issue with resize of the tab labels
   */
  this->label_table_video->Invalidate();
  this->table_video->Invalidate();
  this->label_table_account->Invalidate();
  this->table_account->Invalidate();
  this->notebook->Invalidate();
  this->window_settings->Invalidate();
}

void Settings::build_ui()
{
  // Back button
  this->img_back = sfg::Image::Create();
  sf::Image img;
  if(img.loadFromFile(this->ui->img_path + "back.png"))
    {
      this->img_back->SetImage(img);
      this->img_back->GetSignal(sfg::Widget::OnLeftClick).Connect(&Ui::switch_to_login, this->ui);
      this->img_back->Show(false);
    }
  else
    log_error("cant load img : settings");
  // Label/ComboBox resolution
  this->build_resolution();
  this->label_display = sfg::Label::Create(tr("Display"));
  this->label_display->SetId("label_display");
  // ComboBox
  this->display = sfg::ComboBox::Create();
  this->display->AppendItem(tr("Windowed"));
  this->display->AppendItem(tr("Windowed (fullscreen)"));
  this->display->AppendItem(tr("Fullscreen"));
  this->display->SelectItem(Config::get_int("display", 0));
  this->change_resolution();
  this->display->GetSignal(sfg::ComboBox::OnSelect).Connect(&Settings::change_resolution, this);
  this->label_language = sfg::Label::Create(tr("Language"));
  this->label_language->SetId("label_language");
  this->language = sfg::ComboBox::Create();
  /**
   * @TODO use the language_codes instead of string !
   **/
  this->language->AppendItem(tr("English"));
  this->language->AppendItem(tr("French"));
  this->language->AppendItem(tr("German"));
  this->language->SelectItem(this->get_language());
  // Button event
  this->button_settings_video = sfg::Button::Create(tr("Apply"));
  this->button_settings_video->SetId("button_settings");
  this->button_settings_video->GetSignal(sfg::Widget::OnLeftClick).Connect(&Settings::update, this);
  this->button_settings_account = sfg::Button::Create(tr("Apply"));
  this->button_settings_account->SetId("button_settings");
  this->button_settings_account->GetSignal(sfg::Widget::OnLeftClick).Connect(&Settings::update, this);
  // Table video
  this->table_video = sfg::Table::Create();
  this->table_video->SetRowSpacings(10);
  this->table_video->Attach(this->label_res, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::NORMAL, sfg::Table::NORMAL);
  this->table_video->Attach(this->res, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::NORMAL, sfg::Table::NORMAL);
  this->table_video->SetRowSpacing(1, 30);
  this->table_video->Attach(label_display, sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::NORMAL, sfg::Table::NORMAL);
  this->table_video->Attach(this->display, sf::Rect<sf::Uint32>(0, 3, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND);
  this->table_video->SetRowSpacing(3, 30);
  this->table_video->Attach(button_settings_video, sf::Rect<sf::Uint32>(1, 4, 1, 1), sfg::Table::NORMAL, sfg::Table::NORMAL);
  // Table account
  this->table_account = sfg::Table::Create();
  this->table_account->SetRowSpacings(10);
  this->table_account->Attach(label_language, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::NORMAL, sfg::Table::NORMAL);
  this->table_account->Attach(this->language, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND);
  this->table_account->SetRowSpacing(1, 30);
  this->table_account->Attach(button_settings_account, sf::Rect<sf::Uint32>(1, 2, 1, 1), sfg::Table::NORMAL, sfg::Table::NORMAL);
  // Create the notebook
	this->notebook = sfg::Notebook::Create();
  this->notebook->SetId("notebook_settings");
  this->label_table_video = sfg::Label::Create(tr("Video"));
  this->label_table_account = sfg::Label::Create(tr("Account"));
  this->notebook->AppendPage(this->table_video, this->label_table_video);
  this->notebook->AppendPage(this->table_account, this->label_table_account);
  // Create the window
  this->window_settings = sfg::Window::Create(sfg::Window::NO_STYLE);
  this->window_settings->SetId("window_settings");
  this->window_settings->Add(this->notebook);
  this->window_settings->SetAllocation(sf::FloatRect(300*Config::get_int("width", 600)/1920, 200*Config::get_int("height", 600)/1080, 1200*Config::get_int("width", 600)/1920, 600*Config::get_int("height", 600)/1080));
  this->window_settings->Show(false);
  // Desktop
  this->desktop->Add(this->img_back);
  this->desktop->Add(this->window_settings);
}

int Settings::get_language()
{
  std::string language_conf = Config::get("language", "EN");
  std::string language_selected;
  std::map<std::string, std::string>::iterator it = this->language_codes.begin();
  /**
   * @TODO check if there is a method in std::map to retrieve the key based on the value
   **/
  for (; it != this->language_codes.end(); it++)
  {
    if (it->second.compare(language_conf) == 0)
    {
      language_selected = it->first;
      break;
    }
  }
  for (uint x = 0; x != this->language->GetItemCount(); x++)
    {
      if (this->language->GetItem(x).toAnsiString().compare(language_selected) == 0)
        return x;
    }
  return 0;
}

void Settings::build_resolution()
{
  /**
   *  @todo for default resolution :use the method getDesktopMode () instead of selectioning 800x600 by default.
   **/
  std::ostringstream res_tmp;
  std::string res_config = Config::get("width", "800") + "x" + Config::get("height", "600");
  this->label_res = sfg::Label::Create(tr("Resolution"));
  this->label_res->SetId("label_res");
  this->res = sfg::ComboBox::Create();
  std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

  for (std::size_t i = 0; i < modes.size(); ++i)
    {
      if (modes[i].bitsPerPixel != 32 || modes[i].width < 800)
        break;
      res_tmp << modes[i].width << "x" << modes[i].height;
      this->res->AppendItem(res_tmp.str());
      if (res_config.compare(res_tmp.str()) == 0)
        this->res->SelectItem(i);
      res_tmp.str("");
    }
}

void Settings::update()
{
  log_error("update");
  std::vector<std::string> choosen_res;
  int width;
  int height;
  std::string lang_code;
  std::string selected_text = this->res->GetSelectedText().toAnsiString();

  boost::split(choosen_res, selected_text, boost::is_any_of("x"));
  if (this->display->GetSelectedItem() == 1)
    {
      sf::VideoMode desktop_res = sf::VideoMode::getDesktopMode();
      width = desktop_res.width;
      height = desktop_res.height;
      log_error(width << "x" << height);
    }
  else
    {
      width = atoi(choosen_res.at(0).c_str());
      height = atoi(choosen_res.at(1).c_str());
    }
  try
    {
      lang_code = this->language_codes.at(this->language->GetSelectedText().toAnsiString());
    }
  catch (std::out_of_range)
    {
      log_error("Couldn't select this language !");
    }
  if (!lang_code.empty() && Config::get("language", "EN").compare(lang_code) != 0)
    {
      Config::set("language", lang_code, true);
      this->ui->set_language(lang_code);
      this->ui->translate();
    }
  if ((width != Config::get_int("width", 800)) ||
      (height != Config::get_int("height", 600)) ||
      (this->display->GetSelectedItem() != static_cast<uint>(Config::get_int("display", 2))))
    {
      sf::VideoMode res_new(width, height, 32);
      if (res_new.isValid())
        {
          Config::set_int("width", width);
          Config::set_int("height", height);
          Config::set_int("display", this->display->GetSelectedItem(), true);
          this->window->create(res_new, "Batajelo", this->ui->get_display_mode());
          this->ui->on_resize();
        }
      else
        log_error("display error resolution not handled");
    }
}

void Settings::build_languages()
{
  this->language_codes.insert(std::pair<std::string, std::string>(tr("French"), "FR"));
  this->language_codes.insert(std::pair<std::string, std::string>(tr("English"), "EN"));
  this->language_codes.insert(std::pair<std::string, std::string>(tr("German"), "DE"));
}
