CC = gcc
SRC  = src
LIB  = lib
OBJ  = obj
BIN  = bin
INCLUDE = include

SOURCES = $(wildcard $(SRC)/*.c)
INCLUDES = $(wildcard $(INCLUDE)/*.h)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))


all: $(OBJECTS)
	$(CC) $^ -o  $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -I$(INCLUDE)/ -c $< -o $@

