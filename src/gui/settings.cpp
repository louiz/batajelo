#include <gui/settings.hpp>
#include <gui/ui.hpp>
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

void Settings::deactivate()
{
  this->desktop->Remove(this->img_back);
  this->desktop->Remove(this->window_settings);
}

void Settings::activate()
{
  this->desktop->Add(this->img_back);
  this->desktop->Add(this->window_settings);
}

void Settings::update() 
{
  std::vector<std::string> choosen_res;
  int width;
  int height;

  boost::split(choosen_res, this->res->GetSelectedText().toAnsiString(), boost::is_any_of("x"));
  width = atoi(choosen_res.at(0).c_str());
  height = atoi(choosen_res.at(1).c_str());
  sf::VideoMode res_new(width, height, 32);
  if (res_new.isValid())
  {
    Config::set_int("display", this->display->GetSelectedItem());
    Config::set_int("width", width);
    Config::set_int("height", height, true);
    this->window->create(res_new, "Batajelo", this->ui->get_display_mode());
  }
  else
    log_error("display error resolution not handled");
}

void Settings::draw_background()
{
  sf::Texture bg;
  if (!bg.loadFromFile(this->ui->img_path + Config::get("home_bg", "bg.jpg").data()))
  {
    log_error("cant load home background");
  }
  this->sprite_bg.setTexture(bg);
  this->sprite_bg.setScale((float)Config::get_int("width", 800)/1920, (float)Config::get_int("height", 600)/1080);
  this->window->draw(this->sprite_bg);
}

void Settings::build_ui()
{
  // Back button
  this->img_back = sfg::Image::Create();
  if(this->img.loadFromFile(this->ui->img_path + "back.png"))
  {
    this->img_back->SetImage(this->img);
    this->img_back->SetAllocation(sf::FloatRect(10, 10, 10, 10));
    this->img_back->GetSignal(sfg::Widget::OnLeftClick).Connect(&Ui::switch_to_home, this->ui);
    this->img_back->Show(false);
  }
  else
  {
    log_error("cant load img : settings");
  }

  // Resolution comboBox
  this->build_resolution();

  // Display mode comboBox
  sfg::Label::Ptr label_display = sfg::Label::Create("Display : ");
  label_display->SetId("label_display");
  this->display = sfg::ComboBox::Create();
  this->display->AppendItem("windowed");
  this->display->AppendItem("windowed (fullscreen)");
  this->display->AppendItem("fullscreen");
  this->display->SelectItem(Config::get_int("display", 0));

  // Button event
  sfg::Button::Ptr button_settings(sfg::Button::Create("Ok"));
  button_settings->SetId("button_settings");
  button_settings->GetSignal(sfg::Widget::OnLeftClick).Connect(&Settings::update, this);

  // Create box
  sfg::Box::Ptr box_settings(sfg::Box::Create(sfg::Box::VERTICAL));
  box_settings->SetId("box_settings");
  box_settings->Pack(this->label_res);
  box_settings->Pack(this->res);
  box_settings->Pack(label_display);
  box_settings->Pack(this->display);
  box_settings->SetSpacing(10);
  box_settings->Pack(button_settings);

  // Settings window
  this->window_settings = sfg::Window::Create(sfg::Window::BACKGROUND);
  this->window_settings->SetId("window_settings");
  this->window_settings->Add(box_settings);
  this->window_settings->Show(false);
  this->window_settings->SetAllocation(sf::FloatRect(320, 80, 200, 200));
}

void Settings::build_resolution()
{
  /**
   *  @todo for default resolution :use the method getDesktopMode () instead of selectioning 800x600 by default.
   **/
  std::ostringstream res_tmp;
  std::string res_config = Config::get("width", "800") + "x" + Config::get("height", "600");
  log_error(Config::get("width", "800") + "x" + Config::get("height", "600"));
  this->label_res = sfg::Label::Create("Resolution : ");
  this->res = sfg::ComboBox::Create();
  std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

  this->label_res->SetId("label_res");
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