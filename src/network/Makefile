all: server client

server:
	g++ command_handler.cpp server.cpp remote_client.cpp ../logging/logging.cpp ../config/config.cpp -lboost_system -L/usr/include/boost -lpthread -o server

client:
	g++ client.cpp -lboost_system -L/usr/include/boost ../logging/logging.cpp ../config/config.cpp -lpthread -o client


.PHONY: server client
