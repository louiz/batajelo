#include <network_objects/team.hpp>
#include <database/db_object.hpp>

Team::Team()
{
}

Team::Team(const std::string& id, const std::string& mode,
           const std::string& ladder,  const std::string& season,
           const std::string& win, const std::string& loss,
           const std::vector<Player*>& mates):
id(id), mode(mode), ladder(ladder), season(season), win(win), loss(loss),
mates(mates)
{
}

Team::~Team()
{
}
