#include <database/request.hpp>
#include <database/database.hpp>
#include <logging/logging.hpp>

Request::Request() {}

Request::~Request() {}


DbObject* Request::get(const DbObject* sender, const DbObject* receiver)
{
  std::string where = "receiver_id = " + receiver->get("id");
  where += " AND sender_id = " + sender->get("id");
  where += " AND request_id = " + this->request_id;

  DbObject* request_pending = Database::inst()->get_object("request_id, sender_id, receiver_id", "user_request", where);
  return request_pending;
}

void Request::create(const DbObject* sender, const DbObject* receiver)
{
  DbObject* request_pending = this->get(sender, receiver);

  if (request_pending != NULL)
    {
      log_error("This request is pending ! GO BAN LES FDP DE SPAMMER");
    }
  else
    {
      DbObject* request = new DbObject();
      request->set("sender_id", sender->get("id"));
      request->set("receiver_id", receiver->get("id"));
      request->set("request_id", this->request_id);

      if (Database::inst()->update(request, "user_request") != true)
        log_error("Couldn't register the request, please try again !");
    }
}

void Request::deny(const DbObject* receiver, const DbObject* sender)
{
  DbObject* request_pending = this->get(sender, receiver);

  if (request_pending == NULL)
    {
      log_warning("Friendship request not found !");
    }
  else
    {
      if (Database::inst()->remove(request_pending, "user_request") == false)
        log_error("Couldnt remove the friendship request !");
    }
}
