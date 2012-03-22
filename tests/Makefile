CC =		g++
CFLAGS+=
LFLAGS +=  -lboost_unit_test_framework -lboost_date_time
MYSQL_FLAGS := $(shell mysql_config --libs)  $(shell mysql_config --cflags)

DATABASE_SRC_DIR=	../src/database/
LOGGING_SRC_DIR=	../src/logging/
CONFIG_SRC_DIR=	../src/config/

INCLUDE_DIR= ../src/

SRC =		./database.cpp \
		$(DATABASE_SRC_DIR)database.cpp \
		$(DATABASE_SRC_DIR)db_object.cpp \
		$(DATABASE_SRC_DIR)user.cpp \
		$(LOGGING_SRC_DIR)logging.cpp \
		$(CONFIG_SRC_DIR)config.cpp

OBJ =		$(SRC:.cpp=.o)

NAME = database_test

all: $(SRC) $(NAME) run

$(NAME): $(OBJ)
	g++ $(OBJ) $(LFLAGS) $(MYSQL_FLAGS) -o $@

clean:
	rm -rf $(OBJ)

run: $(NAME)
	./$(NAME)
.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCLUDE_DIR) $(MYSQL_FLAGS) $< -o $@
