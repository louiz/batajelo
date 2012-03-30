#include <config/config.hpp>
#include <database/database.hpp>
#include <database/friend_request.hpp>
#include <logging/logging.hpp>

FriendRequest::FriendRequest()
{
  request_id = Config::get("request_friendship", "1");
}

FriendRequest::~FriendRequest()
{
}

void FriendRequest::accept(const DbObject* receiver, const DbObject* sender)
{
  DbObject* request_pending = this->get(sender, receiver);

  if (request_pending == NULL)
    {
      log_error("Request not found !");
    }
  else
    {
      DbObject* friendship = new DbObject();
      friendship->set("user_id", sender->get("id"));
      friendship->set("friend_id", receiver->get("id"));
      if (Database::inst()->update(friendship, "user_friend") != true)
        {
          log_error("Couldn't execute the request, please try again !");
        }
      else
        {
          if (Database::inst()->remove(request_pending, "user_request") == false)
            log_error("Couldn't remove the user_request !");
        }
   }
}
