all: database

MYSQL_FLAGS	:= $(shell mysql_config --libs)  $(shell mysql_config --cflags)

database:
	g++ db_object.cpp database.cpp -o database $(MYSQL_FLAGS) ../config/config.cpp ../logging/logging.cpp main.cpp

.PHONY: database
