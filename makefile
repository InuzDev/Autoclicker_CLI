CC = gcc
CFLAGS = -std=gnu17 -Wall -Wextra

SRC = Autoclicker.c
TARGET = Build/Autoclicker

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
