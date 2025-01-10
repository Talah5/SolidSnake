# Makefile for SolidSnake

CC = gcc
CFLAGS = -Wall -Wextra -pthread -g

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = solid_snake

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
