#include <database/team.hpp>
#include <database/database.hpp>
#include <logging/logging.hpp>


Team::Team() {}

Team::~Team() {}

void Team::add_team()
{
  std::vector<DbObject*>::iterator team_it;
  DbObject* team = new DbObject();
  if (Database::inst()->update(team, "team") != true)
    {
      log_error("Couldn't add a new row in table team");
    }

}

void Team::add_user(const std::string& mode, const std::string& race)
{
  DbObject* team_user = new DbObject();
  team_user->set("team_id", "LAST_INSERT_ID()");
  team_user->set("user_id", this->get("id"));
  team_user->set("race_id", race);
  team_user->set("mode_id", mode);
}