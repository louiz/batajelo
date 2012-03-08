all: server client

server:
	g++ server.cpp remote_client.cpp -lboost_system -L/usr/include/boost -lpthread -o server

client:
	g++ client.cpp -lboost_system -L/usr/include/boost -lpthread -o client


.PHONY: server client
