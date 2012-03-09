all: database

database:
	g++ user.cpp database.cpp -o database $(mysql_config --libs) $(mysql_config --cflags) -W -Wall

.PHONY: database
