#include <network/message.hpp>
#include <network/message_handler.hpp>
#include <network/tcp_socket.hpp>
#include <network/client_base.hpp>
#include <network/remote_client_base.hpp>
#include <network/server.hpp>

#include "requests.pb.h"
#include "catch.hpp"

TEST_CASE("message_consistency_test1", "[network]")
{
  Message message;
  message.set_body("coucou les amis");
  message.set_name("FAKE_MESSAGE");
  message.pack();

  CHECK(message.header == std::string("FAKE_MESSAGE.15:"));
  CHECK(message.body_size == 15);
  CHECK(strncmp(message.body, "coucou les amis", 15) == 0);
}

TEST_CASE("message_consistency_test2", "[network]")
{
  Message message;
  message.set_body("");
  message.set_name("G");
  message.pack();

  CHECK(message.header == std::string("G:"));
  CHECK(message.body_size == 0);
}

TEST_CASE("message_consistency_test3", "[network]")
{
  Message message;
  message.set_body("");
  message.set_name("GUGU");
  message.pack();

  CHECK(message.header == std::string("GUGU.:"));
  CHECK(message.body_size == 0);
  CHECK(message.get_name() == "GUGU");
}

TEST_CASE("message_consistency_test4", "[network]")
{
  Message message;
  message.set_body("salut");
  message.set_name("G");
  message.pack();

  CHECK(message.header == std::string("G5:"));
  CHECK(message.body_size == 5);
  CHECK(strncmp(message.body, "salut", 5) == 0);
}

TEST_CASE("message_copy_test", "[network]")
{
  Message message;
  message.set_body("coucou les amis");
  message.set_name("FAKE_MESSAGE");
  message.pack();

  Message message2(message);
  message2.pack();
  CHECK(message2.header == std::string("FAKE_MESSAGE.15:"));
  CHECK(message2.body_size == 15);
  CHECK(strncmp(message2.body, "coucou les amis", 15) == 0);
}

TEST_CASE("very long message", "[network]")
{
  Message message;
  constexpr size_t body_size = 133700;
  const std::string body(body_size, 'a');
  message.set_body(body.data(), body.size());
  message.set_name("TEST");
  message.pack();
  CHECK(message.body_size == body_size);
  CHECK(message.header == "TEST.133700:");
}

TEST_CASE("message from protobuf struct", "[network]")
{
  Message message;
  message.set_name("TEST");

  ser::request::Cast req;
  req.add_entity_id(12);
  req.set_queue(true);
  req.mutable_pos()->set_x(8);
  req.mutable_pos()->set_y(9);
  WHEN("the struct is not complete")
    {
      req.clear_type();
      THEN("the struct cannot be used as a message body")
        {
          CHECK_THROWS_AS(message.set_body(req), const SerializationException&);
        }
    }
  AND_WHEN("the struct is complete")
    {
      req.set_type(12);
      THEN("everything is fine")
        {
          message.set_body(req);
          message.pack();
          CHECK(message.body_size > 0);
          AND_THEN("we can de-serialize the message into a protobuf struct")
            {
              auto result = message.parse_body_to_protobuf_object<ser::request::Cast>();
              CHECK(result.pos().x() == 8);
              CHECK(result.pos().y() == 9);
              CHECK(result.queue());
            }
        }
    }
  WHEN("we receive a malformed message")
    {
      message.set_body("GARBAGE");
      THEN("trying to deserialize into a protobuf structure fails")
        {
          CHECK_THROWS_AS(message.parse_body_to_protobuf_object<ser::request::Cast>(), const SerializationException&);
        }
    }
}

class TestRemoteClient: public RemoteClientBase<TCPSocket>
{
 public:
  static bool message_was_transfered;
  static bool connection_failed;
  static std::function<void(Message*)> check_message;

 private:
  void install_callbacks()
  {
    this->install_callback("REQUEST", [this](Message* message)
    {
      CHECK(message->get_name() == "REQUEST");
      if (TestRemoteClient::check_message)
        TestRemoteClient::check_message(message);
      TestRemoteClient::message_was_transfered = true;
      IoService::get().stop();
    });
    this->install_callback("PING_REQUEST", [this](Message* message)
    {
      this->send_ping();
    });
  }
  virtual void on_connection_closed() override
  {
    TestRemoteClient::connection_failed = true;
  }
};

bool TestRemoteClient::message_was_transfered = false;
bool TestRemoteClient::connection_failed = false;
std::function<void(Message*)> TestRemoteClient::check_message = nullptr;

class SingleClientServer: public Server<TestRemoteClient>
{
 public:
  using Server<TestRemoteClient>::Server;
  /**
   * We stop accepting new clients after the first one.
   */
  virtual void on_new_client(TestRemoteClient*) override final
  {
    this->stop();
  }
  ~SingleClientServer() = default;
  SingleClientServer& operator=(const SingleClientServer&) = delete;
  SingleClientServer& operator=(SingleClientServer&&) = delete;
  SingleClientServer(const SingleClientServer&) = delete;
  SingleClientServer(SingleClientServer&&) = delete;

 private:
};

SCENARIO("Sending a Message from a MessageHandler to an other", "[network]")
{
  GIVEN("A client and a simple listening server")
    {
      IoService::get().reset();
      TestRemoteClient::message_was_transfered = false;
      TestRemoteClient::connection_failed = false;
      TestRemoteClient::check_message = nullptr;
      ClientBase<TCPSocket> client;
      SingleClientServer server(18898);
      server.start();

      WHEN("the client connects to the server using TCP and sends a message")
        {
          client.connect("127.0.0.1", 18898, [&client](const boost::system::error_code& error)
          {
            CHECK(!error);
            client.send_message("REQUEST", "body content");
            TestRemoteClient::check_message = [](Message* message)
            {
              const std::string body(message->body, message->body_size);
              CHECK(body == "body content");
            };
          });
          IoService::get().run();
          THEN("the message is received by a remote_client")
            {
              CHECK(TestRemoteClient::message_was_transfered);
              CHECK(!TestRemoteClient::connection_failed);
            }
        }

      WHEN("the client connects to a closed port")
        {
          client.connect("127.0.0.1", 7778, [&server](const boost::system::error_code& error)
          {
            CHECK(error);
            TestRemoteClient::connection_failed = true;
            server.stop();
          });
          IoService::get().run();
          THEN("the connection fails and the message is not received")
            {
              CHECK(TestRemoteClient::connection_failed);
              CHECK(!TestRemoteClient::message_was_transfered);
            }
        }

      WHEN("the client connects to the server using TCP and sends a very very long message")
        {
          client.connect("127.0.0.1", 18898, [&client](const boost::system::error_code& error)
          {
            CHECK(!error);
            const std::string body(133700, 'a');
            client.send_message("REQUEST", body);
            TestRemoteClient::check_message = [](Message* message)
            {
              CHECK(message->body_size == 133700);
            };
          });
          IoService::get().run();
          THEN("the message is received by a remote_client")
            {
              CHECK(TestRemoteClient::message_was_transfered);
              CHECK(!TestRemoteClient::connection_failed);
            }
        }

      WHEN("the client sends a message with a callback")
        {
          bool sent = false;
          client.connect("127.0.0.1", 18898, [&sent, &client](const boost::system::error_code& error)
          {
            CHECK(!error);
            Message* message = new Message;
            message->set_name("REQUEST");
            message->callback = [&sent]()
            { sent = true; };
            client.send(message);
          });
          IoService::get().run();
          THEN("the callback is called")
          {
            CHECK(sent);
          }
        }
    }
}

