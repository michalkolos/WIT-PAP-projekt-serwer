CC 			= gcc
SRC  		= src
LIB  		= lib
OBJ  		= obj
BIN  		= bin
INCLUDE 	= include
EXECUTABLE 	= run_server.bin

SOURCES  = $(wildcard $(SRC)/*.c)
INCLUDES = $(wildcard $(INCLUDE)/*.h)
OBJECTS  = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))


all: $(OBJECTS)
	$(CC) $^ -o  $(BIN)/$(EXECUTABLE) -lpthread

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -I$(INCLUDE)/ -c $< -o $@ 

