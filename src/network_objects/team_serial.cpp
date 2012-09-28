#include <network_objects/team_serial.hpp>
#include <database/db_object.hpp>

TeamSerial::TeamSerial(const std::vector<DbObject*>& teams)
{
  static const std::string modes[] = {"1v1", "2v2", "3v3", "4v4"};
  std::vector<DbObject*>::const_iterator it = teams.begin();
  std::vector<DbObject*>::const_iterator next = teams.begin();
  next++;
  std::vector<Player*> players;
  Player* player;
  Team* team = new Team();
  for (; it != teams.end(); it++)
    {
      if (next != teams.end())
        next++;
      player = new Player();
      log_error((*it)->get("team_id"));
      player->id = (*it)->get("user_id");
      player->login = (*it)->get("login");
      player->race = (*it)->get("race");
      log_error(player->id << " " << player->login << " " << player->race);
      team->mates.push_back(player);
      log_error("a");
      if (next == teams.end() || (*it)->get("team_id") != (*next)->get("team_id"))
        {
          log_error((*it)->get("team_id"));
          team->id = (*it)->get("team_id");
          team->mode = modes[team->mates.size() - 1];
          log_error((*it)->get("ladder"));
          team->ladder = (*it)->get("ladder");
          log_error((*it)->get("season"));
          team->season = (*it)->get("season");
                    log_error((*it)->get("win"));
          team->win = (*it)->get("win");
                    log_error((*it)->get("loss"));
          team->loss = (*it)->get("loss");
          log_error("TEAM id : " << team->id);
          this->teams.push_back(team);
          team = new Team();
        }
  }
}

TeamSerial::TeamSerial()
{
}

TeamSerial::~TeamSerial()
{
}
