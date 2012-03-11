all: database

MYSQL_FLAGS	= $(shell mysql_config --libs)  $(shell mysql_config --cflags)
database:
	g++ user.cpp database.cpp -o database -W -Wall $(MYSQL_FLAGS)

.PHONY: database
