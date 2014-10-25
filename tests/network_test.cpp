#include <network/message.hpp>

#include <gtest/gtest.h>

TEST(Network, message_consistency_test1)
{
  Message* message = new Message;
  message->set_body("coucou les amis");
  message->set_name("FAKE_MESSAGE");
  message->pack();

  EXPECT_EQ(message->header, std::string("FAKE_MESSAGE.15:"));
  EXPECT_EQ(message->body_size, 15);
  EXPECT_EQ(strncmp(message->body, "coucou les amis", 15), 0);

  delete message;
}

TEST(Network, message_consistency_test2)
{
  Message* message = new Message;
  message->set_body("");
  message->set_name("G");
  message->pack();

  EXPECT_EQ(message->header, std::string("G:"));
  EXPECT_EQ(message->body_size, 0);
  EXPECT_EQ(strncmp(message->body, "", 0), 0);

  delete message;
}

TEST(Network, message_consistency_test3)
{
  Message* message = new Message;
  message->set_body("");
  message->set_name("GUGU");
  message->pack();

  EXPECT_EQ(message->header, std::string("GUGU.:"));
  EXPECT_EQ(message->body_size, 0);
  EXPECT_EQ(strncmp(message->body, "", 0), 0);

  delete message;
}

TEST(Network, message_consistency_test4)
{
  Message* message = new Message;
  message->set_body("salut");
  message->set_name("G");
  message->pack();

  EXPECT_EQ(message->header, std::string("G5:"));
  EXPECT_EQ(message->body_size, 5);
  EXPECT_EQ(strncmp(message->body, "salut", 5), 0);

  delete message;
}

TEST(Network, message_copy_test)
{
  Message* message = new Message;
  message->set_body("coucou les amis");
  message->set_name("FAKE_MESSAGE");
  message->pack();

  Message* message2 = new Message(*message);
  message2->pack();
  EXPECT_EQ(message2->header, std::string("FAKE_MESSAGE.15:"));
  EXPECT_EQ(message2->body_size, 15);
  EXPECT_EQ(strncmp(message2->body, "coucou les amis", 15), 0);

  delete message;
  delete message2;
}
